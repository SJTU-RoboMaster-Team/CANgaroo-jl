/*
  Copyright (c) 2026 SJTU RoboMaster Team

  This file is part of cangaroo.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TriggerCaptureManager.h"

#include <cmath>
#include <span>
#include <utility>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>

#include "core/Log.h"
#include "core/TraceFileWriter.h"

TriggerCaptureManager::TriggerCaptureManager(InterfaceNameFn interfaceName,
                                             SignalValueFn signalValueFn,
                                             QObject *parent)
    : QObject(parent),
      _interfaceName(std::move(interfaceName)),
      _signalValueFn(std::move(signalValueFn))
{
    _pollTimer.setInterval(k_pollIntervalMs);
    connect(&_pollTimer, &QTimer::timeout, this, &TriggerCaptureManager::onPollTick);
}

TriggerCaptureManager::~TriggerCaptureManager()
{
    flushPendingCaptures();
}

void TriggerCaptureManager::setConfig(const TriggerCaptureConfig &config)
{
    _config = config;
    if (_config.preTriggerSeconds < 0.0) _config.preTriggerSeconds = 0.0;
    if (_config.postTriggerSeconds < 0.0) _config.postTriggerSeconds = 0.0;
    if (_config.maxPreTriggerFrames < 100) _config.maxPreTriggerFrames = 100;
    if (_config.holdoffMs < 0) _config.holdoffMs = 0;
    _watchdogLastSeenUs.clear();
    _watchdogFired.clear();
}

void TriggerCaptureManager::setArmed(bool armed)
{
    const bool prev = _armed.exchange(armed, std::memory_order_relaxed);
    if (prev == armed)
    {
        return;
    }

    if (armed)
    {
        _watchdogLastSeenUs.clear();
        _watchdogFired.clear();
        _lastTriggerTimeUs = 0;
        if (!_pollTimer.isActive())
        {
            _pollTimer.start();
        }
    }
    else
    {
        processNow();
        flushPendingCaptures();
        _pollTimer.stop();
        resetState();
    }

    emit armedChanged(armed);
}

void TriggerCaptureManager::enqueue(const BusMessage &msg)
{
    if (!_armed.load(std::memory_order_relaxed))
    {
        return;
    }
    QMutexLocker locker(&_pendingMutex);
    if (_pending.size() >= k_maxPending)
    {
        return;
    }
    _pending.append(msg);
}

void TriggerCaptureManager::resetState()
{
    {
        QMutexLocker locker(&_pendingMutex);
        _pending.clear();
    }
    _preRing.clear();
    _preHead = 0;
    _captureFrames.clear();
    _captureReason.clear();
    _capturingPost = false;
    _watchdogLastSeenUs.clear();
    _watchdogFired.clear();
    _latestTimestampUs = 0;
}

void TriggerCaptureManager::onPollTick()
{
    processNow();
    if (_armed.load(std::memory_order_relaxed) && _latestTimestampUs > 0)
    {
        // Use wall-clock advance if bus went completely silent
        const int64_t nowUs = QDateTime::currentMSecsSinceEpoch() * 1000LL;
        checkWatchdogsAt(qMax(_latestTimestampUs, nowUs));
    }
}

void TriggerCaptureManager::processNow()
{
    QVector<BusMessage> batch;
    {
        QMutexLocker locker(&_pendingMutex);
        batch = std::exchange(_pending, {});
    }

    for (const BusMessage &msg : std::as_const(batch))
    {
        const int64_t tsUs = msg.getTimestamp_us();
        if (tsUs > _latestTimestampUs)
        {
            _latestTimestampUs = tsUs;
        }

        // Update watchdog last-seen timestamps for matching CAN IDs
        if (!msg.isErrorFrame())
        {
            for (int i = 0; i < _config.rules.size(); ++i)
            {
                const TriggerRule &r = _config.rules[i];
                if (!r.enabled || r.kind != TriggerRule::Kind::Watchdog) continue;
                if (r.interfaceId != InvalidBusInterfaceId && r.interfaceId != msg.getInterfaceId()) continue;
                if (msg.getId() >= r.idMin && msg.getId() <= r.idMax)
                {
                    _watchdogLastSeenUs[i] = tsUs;
                    _watchdogFired[i] = false;
                }
            }
        }

        appendToPreRing(msg);

        if (_capturingPost)
        {
            _captureFrames.append(msg);
            if ((msg.getFloatTimestamp() - _triggerTimestampSec) >= _config.postTriggerSeconds)
            {
                finalizeCapture();
                if (_config.oneShot)
                {
                    setArmed(false);
                    return;
                }
            }
            continue;
        }

        if (!_armed.load(std::memory_order_relaxed))
        {
            continue;
        }

        // Respect holdoff between successive triggers
        if (_lastTriggerTimeUs > 0
            && (tsUs - _lastTriggerTimeUs) < static_cast<int64_t>(_config.holdoffMs) * 1000LL)
        {
            continue;
        }

        const QString reason = evaluateFrameRules(msg);
        if (!reason.isEmpty())
        {
            startPostCapture(reason, msg.getFloatTimestamp(), tsUs);
            if (_config.postTriggerSeconds <= 0.0)
            {
                finalizeCapture();
                if (_config.oneShot)
                {
                    setArmed(false);
                    return;
                }
            }
        }
    }

    if (_latestTimestampUs > 0)
    {
        checkWatchdogsAt(_latestTimestampUs);
    }
}

void TriggerCaptureManager::checkWatchdogsAt(int64_t nowUs)
{
    if (!_armed.load(std::memory_order_relaxed))
    {
        return;
    }

    if (_capturingPost)
    {
        const double nowSec = static_cast<double>(nowUs) / 1e6;
        if ((nowSec - _triggerTimestampSec) >= _config.postTriggerSeconds)
        {
            finalizeCapture();
            if (_config.oneShot)
            {
                setArmed(false);
            }
        }
        return;
    }

    if (_lastTriggerTimeUs > 0
        && (nowUs - _lastTriggerTimeUs) < static_cast<int64_t>(_config.holdoffMs) * 1000LL)
    {
        return;
    }

    for (int i = 0; i < _config.rules.size(); ++i)
    {
        const TriggerRule &r = _config.rules[i];
        if (!r.enabled || r.kind != TriggerRule::Kind::Watchdog) continue;

        const auto it = _watchdogLastSeenUs.constFind(i);
        if (it == _watchdogLastSeenUs.cend() || it.value() <= 0) continue;
        if (_watchdogFired.value(i, false)) continue;

        const int64_t silentUs = nowUs - it.value();
        if (silentUs >= static_cast<int64_t>(r.watchdogTimeoutMs) * 1000LL)
        {
            _watchdogFired[i] = true;
            const QString label = r.name.isEmpty()
                ? tr("Watchdog timeout on 0x%1 (> %2 ms)").arg(r.idMin, 0, 16).arg(r.watchdogTimeoutMs)
                : r.name;
            startPostCapture(label, static_cast<double>(nowUs) / 1e6, nowUs);
            if (_config.postTriggerSeconds <= 0.0)
            {
                finalizeCapture();
                if (_config.oneShot)
                {
                    setArmed(false);
                }
            }
            return;
        }
    }
}

void TriggerCaptureManager::flushPendingCaptures()
{
    processNow();
    if (_capturingPost)
    {
        finalizeCapture();
    }
}

void TriggerCaptureManager::appendToPreRing(const BusMessage &msg)
{
    _preRing.append(msg);

    const double cutoff = msg.getFloatTimestamp() - _config.preTriggerSeconds;
    const qsizetype maxFrames = static_cast<qsizetype>(_config.maxPreTriggerFrames);
    while (_preHead < _preRing.size() - 1)
    {
        const qsizetype liveCount = _preRing.size() - _preHead;
        if (liveCount > maxFrames || _preRing[_preHead].getFloatTimestamp() < cutoff)
        {
            ++_preHead;
        }
        else
        {
            break;
        }
    }

    if (_preHead > 0 && (_preHead * 2 >= _preRing.size() || _preHead > 4096))
    {
        _preRing.remove(0, _preHead);
        _preHead = 0;
    }
}

bool TriggerCaptureManager::matchesCompare(double lhs, TriggerRule::CompareOp op, double rhs) const noexcept
{
    switch (op)
    {
        case TriggerRule::CompareOp::Equal:        return std::fabs(lhs - rhs) < 1e-9;
        case TriggerRule::CompareOp::NotEqual:     return std::fabs(lhs - rhs) >= 1e-9;
        case TriggerRule::CompareOp::Greater:      return lhs > rhs;
        case TriggerRule::CompareOp::Less:         return lhs < rhs;
        case TriggerRule::CompareOp::GreaterEqual: return lhs >= rhs;
        case TriggerRule::CompareOp::LessEqual:    return lhs <= rhs;
    }
    return false;
}

QString TriggerCaptureManager::evaluateFrameRules(const BusMessage &msg)
{
    for (const TriggerRule &r : std::as_const(_config.rules))
    {
        if (!r.enabled) continue;
        if (r.interfaceId != InvalidBusInterfaceId && r.interfaceId != msg.getInterfaceId()) continue;

        if (r.kind == TriggerRule::Kind::BusErrorEvent)
        {
            if (!msg.isErrorFrame()) continue;
            if (r.errorMask == BusErrors{} || (msg.errorFlags() & r.errorMask) != BusErrors{})
            {
                return r.name.isEmpty() ? tr("Bus error frame triggered") : r.name;
            }
            continue;
        }

        if (r.kind == TriggerRule::Kind::FrameMatch)
        {
            if (msg.isErrorFrame()) continue;
            if (r.direction == TriggerRule::Direction::RxOnly && !msg.isRX()) continue;
            if (r.direction == TriggerRule::Direction::TxOnly && msg.isRX()) continue;
            if (msg.getId() < r.idMin || msg.getId() > r.idMax) continue;

            if (r.byteIndex >= 0)
            {
                if (r.byteIndex >= msg.getLength()) continue;
                const uint8_t masked = static_cast<uint8_t>(msg.getByte(static_cast<uint8_t>(r.byteIndex)) & r.byteMask);
                if (!matchesCompare(static_cast<double>(masked), r.byteOp, static_cast<double>(r.byteValue)))
                {
                    continue;
                }
            }
            return r.name.isEmpty()
                ? tr("Frame match on 0x%1").arg(msg.getId(), 0, 16)
                : r.name;
        }

        if (r.kind == TriggerRule::Kind::SignalThreshold && _signalValueFn)
        {
            if (msg.isErrorFrame()) continue;
            double phys = 0.0;
            if (_signalValueFn(msg, r.signalMessageName, r.signalName, phys)
                && matchesCompare(phys, r.signalOp, r.signalThreshold))
            {
                return r.name.isEmpty()
                    ? tr("Signal %1.%2 = %3").arg(r.signalMessageName, r.signalName).arg(phys, 0, 'f', 3)
                    : r.name;
            }
        }
    }
    return {};
}

void TriggerCaptureManager::startPostCapture(const QString &reason, double triggerSec, int64_t triggerUs)
{
    _capturingPost = true;
    _captureReason = reason;
    _triggerTimestampSec = triggerSec;
    _lastTriggerTimeUs = triggerUs;

    _captureFrames.clear();
    const qsizetype count = _preRing.size() - _preHead;
    _captureFrames.reserve(count + 1024);
    for (qsizetype i = _preHead; i < _preRing.size(); ++i)
    {
        _captureFrames.append(_preRing[i]);
    }

    log_warning(tr("Trigger Capture fired: %1 (pre-buffer: %2 frames)").arg(reason).arg(_captureFrames.size()));
    emit triggered(reason, triggerSec);
}

QString TriggerCaptureManager::nextCapturePath() const
{
    const QString prefix = _config.filePrefix.trimmed().isEmpty() ? QStringLiteral("trigger") : _config.filePrefix.trimmed();
    const QString stamp = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
    const QString ext = traceFormatExtension(_config.format);

    for (int n = 1; ; ++n)
    {
        const QString fileName = QStringLiteral("%1_%2_%3.%4")
                                     .arg(prefix, stamp)
                                     .arg(n, 3, 10, QLatin1Char('0'))
                                     .arg(ext);
        const QString fullPath = QDir(_config.outputFolder).filePath(fileName);
        if (!QFileInfo::exists(fullPath))
        {
            return fullPath;
        }
    }
}

void TriggerCaptureManager::finalizeCapture()
{
    _capturingPost = false;

    if (_config.outputFolder.isEmpty() || !QDir().mkpath(_config.outputFolder))
    {
        const QString err = tr("Trigger capture failed: cannot create output folder '%1'.").arg(_config.outputFolder);
        log_error(err);
        emit captureFailed(err);
        _captureFrames.clear();
        return;
    }

    const QString path = nextCapturePath();
    const bool binary = (_config.format == TraceFileFormat::Pcap
                         || _config.format == TraceFileFormat::PcapNg
                         || _config.format == TraceFileFormat::VectorMdf);
    QFile file(path);
    if (!file.open(binary ? (QIODevice::WriteOnly | QIODevice::Truncate)
                          : (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)))
    {
        const QString err = tr("Trigger capture failed: cannot open '%1': %2").arg(path, file.errorString());
        log_error(err);
        emit captureFailed(err);
        _captureFrames.clear();
        return;
    }

    const std::span<const BusMessage> span(_captureFrames.constData(), static_cast<std::size_t>(_captureFrames.size()));
    TraceFileWriter::write(file, _config.format, span, _interfaceName);
    file.flush();
    file.close();

    if (file.error() != QFileDevice::NoError)
    {
        const QString err = tr("Trigger capture write error on '%1': %2").arg(path, file.errorString());
        log_error(err);
        emit captureFailed(err);
        _captureFrames.clear();
        return;
    }

    ++_captureCount;
    const int savedCount = static_cast<int>(_captureFrames.size());
    const QString reason = _captureReason;
    _captureFrames.clear();

    log_info(tr("Trigger capture saved %1 frames to %2 (%3)").arg(savedCount).arg(path, reason));
    emit captureSaved(path, savedCount, reason);
}
