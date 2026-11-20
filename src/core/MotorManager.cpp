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

#include "MotorManager.h"

#include <QDateTime>
#include <QMutexLocker>
#include <QTimer>

#include "decoders/ProtocolTypes.h"

MotorManager::MotorManager(QObject *parent)
  : QObject(parent)
{
    _decoder = std::make_shared<MotorDecoder>();
    _protocolManager.addDecoder(_decoder);

    // Frames arrive on the bus listener thread at bus rate; publish a bounded
    // number of updates per second from the owning (GUI) thread instead.
    _refreshTimer = new QTimer(this);
    _refreshTimer->setTimerType(Qt::PreciseTimer);
    _refreshTimer->setInterval(33);
    connect(_refreshTimer, &QTimer::timeout, this, &MotorManager::onRefreshTimer);
    _refreshTimer->start();
}

MotorManager::~MotorManager() = default;

void MotorManager::setMotors(const QVector<MotorConfig> &motors)
{
    {
        QMutexLocker locker(&_mutex);
        _config = motors;
        _decoder->setMotors(motors);
        _protocolManager.reset();
        _states.fill(MotorState(), motors.size());
        _dirty.store(true);
    }
    emit configChanged();
    emit statesUpdated();
}

QVector<MotorConfig> MotorManager::motors() const
{
    QMutexLocker locker(&_mutex);
    return _config;
}

QVector<MotorState> MotorManager::states() const
{
    QMutexLocker locker(&_mutex);
    return _states;
}

bool MotorManager::hasMotors() const
{
    QMutexLocker locker(&_mutex);
    return !_config.isEmpty();
}

void MotorManager::enqueue(const BusMessage &msg)
{
    QMutexLocker locker(&_mutex);
    if (_config.isEmpty()) {
        return;
    }

    ProtocolMessage decoded;
    const DecodeStatus status = _protocolManager.processFrame(msg, decoded);
    if (status != DecodeStatus::Completed) {
        return;
    }

    int index = -1;
    bool ok = false;
    MotorState state = motorStateFromProtocolMessage(decoded, &index, &ok);
    if (!ok || index < 0 || index >= _states.size()) {
        return;
    }

    // A negative feedback CAN ID never matches; the check above is only about
    // the metadata actually describing one of our configured motors.
    const MotorConfig &config = _config.at(index);
    if (config.invert) {
        state.angleDeg = 360.0 - state.angleDeg;
        state.speedRpm = -state.speedRpm;
        state.currentA = -state.currentA;
        state.positionRad = -state.positionRad;
        state.velocityRadPerSec = -state.velocityRadPerSec;
        state.torqueNm = -state.torqueNm;
    }

    state.hasData = true;
    state.online = true;
    state.lastSeenMs = QDateTime::currentMSecsSinceEpoch();
    _states[index] = state;
    _dirty.store(true);
}

void MotorManager::resetStates()
{
    {
        QMutexLocker locker(&_mutex);
        _states.fill(MotorState(), _states.size());
        _dirty.store(true);
    }
    emit statesUpdated();
}

void MotorManager::onRefreshTimer()
{
    bool changed = false;
    {
        QMutexLocker locker(&_mutex);
        const qint64 now = QDateTime::currentMSecsSinceEpoch();
        for (int i = 0; i < _states.size(); ++i) {
            MotorState &state = _states[i];
            const int64_t timeoutMs = i < _config.size()
                ? MotorProtocols::offlineTimeoutMsForModel(_config.at(i).model)
                : offlineTimeoutMs;
            const bool online = state.hasData
                && (now - state.lastSeenMs) <= timeoutMs;
            if (state.online != online) {
                state.online = online;
                changed = true;
            }
        }
        changed = changed || _dirty.exchange(false);
    }

    if (changed) {
        emit statesUpdated();
    }
}
