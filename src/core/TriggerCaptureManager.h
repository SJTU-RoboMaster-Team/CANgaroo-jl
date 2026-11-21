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

#pragma once

#include <atomic>
#include <cstdint>
#include <functional>

#include <QDateTime>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVector>

#include "core/BusMessage.h"
#include "core/TraceFileFormat.h"

struct TriggerRule
{
    enum class Kind
    {
        BusErrorEvent = 0, // Hardware/controller error frame (BusOff, Overrun, Ack, etc.)
        FrameMatch    = 1, // Software frame ID range + optional payload byte mask comparison
        Watchdog      = 2, // Frame loss timeout: target ID silent for > watchdogTimeoutMs
        SignalThreshold = 3 // DBC physical signal threshold via injected evaluator
    };

    enum class Direction
    {
        Any = 0,
        RxOnly = 1,
        TxOnly = 2
    };

    enum class CompareOp
    {
        Equal = 0,
        NotEqual = 1,
        Greater = 2,
        Less = 3,
        GreaterEqual = 4,
        LessEqual = 5
    };

    QString name;
    bool enabled = true;
    Kind kind = Kind::FrameMatch;

    // BusErrorEvent: 0 matches any error frame, otherwise tests BusErrors intersection
    BusErrors errorMask{};

    // FrameMatch & Watchdog
    uint32_t idMin = 0;
    uint32_t idMax = 0x7FF;
    Direction direction = Direction::Any;
    BusInterfaceId interfaceId = InvalidBusInterfaceId; // InvalidBusInterfaceId = any

    // Optional byte check for FrameMatch (byteIndex < 0 disables byte check)
    int byteIndex = -1;
    uint8_t byteMask = 0xFF;
    uint8_t byteValue = 0x00;
    CompareOp byteOp = CompareOp::Equal;

    // Watchdog timeout in ms (for Kind::Watchdog)
    int watchdogTimeoutMs = 200;

    // SignalThreshold (for Kind::SignalThreshold)
    QString signalMessageName;
    QString signalName;
    CompareOp signalOp = CompareOp::Greater;
    double signalThreshold = 0.0;
};

struct TriggerCaptureConfig
{
    QString outputFolder;
    QString filePrefix = QStringLiteral("trigger");
    TraceFileFormat format = TraceFileFormat::Csv;
    double preTriggerSeconds = 5.0;
    double postTriggerSeconds = 5.0;
    int maxPreTriggerFrames = 20000;
    bool oneShot = false;
    int holdoffMs = 2000;
    QVector<TriggerRule> rules;
};

// Bounded pre-trigger ring buffer + post-trigger blackbox slice exporter.
// Thread-safe enqueue() can be called from BusListener threads or ReplayWindow.
class TriggerCaptureManager : public QObject
{
    Q_OBJECT

public:
    using InterfaceNameFn = std::function<QString(BusInterfaceId)>;
    using SignalValueFn   = std::function<bool(const BusMessage &, const QString &msgName,
                                               const QString &sigName, double &outValue)>;

    explicit TriggerCaptureManager(InterfaceNameFn interfaceName,
                                   SignalValueFn signalValueFn = {},
                                   QObject *parent = nullptr);
    ~TriggerCaptureManager() override;

    void setConfig(const TriggerCaptureConfig &config);
    [[nodiscard]] const TriggerCaptureConfig &config() const noexcept { return _config; }

    void setArmed(bool armed);
    [[nodiscard]] bool isArmed() const noexcept { return _armed.load(std::memory_order_relaxed); }
    [[nodiscard]] bool isCapturingPostTrigger() const noexcept { return _capturingPost; }
    [[nodiscard]] int captureCount() const noexcept { return _captureCount; }

    // Thread-safe ingestion from BusTrace::enqueueMessage().
    void enqueue(const BusMessage &msg);

    // Deterministic step for tests and immediate shutdown flush.
    void processNow();
    void checkWatchdogsAt(int64_t nowUs);
    void flushPendingCaptures();
    void resetState();

signals:
    void armedChanged(bool armed);
    void triggered(const QString &reason, double timestampSec);
    void captureSaved(const QString &filePath, int frameCount, const QString &reason);
    void captureFailed(const QString &errorMessage);

private slots:
    void onPollTick();

private:
    static constexpr int k_pollIntervalMs = 50;
    static constexpr qsizetype k_maxPending = 200000;

    InterfaceNameFn _interfaceName;
    SignalValueFn _signalValueFn;
    TriggerCaptureConfig _config;

    std::atomic<bool> _armed{false};
    bool _capturingPost = false;
    int _captureCount = 0;

    QMutex _pendingMutex;
    QVector<BusMessage> _pending;

    // Pre-trigger ring (head index avoids repeated O(n) front erasures)
    QVector<BusMessage> _preRing;
    qsizetype _preHead = 0;

    // Active post-trigger capture accumulation
    QVector<BusMessage> _captureFrames;
    QString _captureReason;
    double _triggerTimestampSec = 0.0;
    int64_t _lastTriggerTimeUs = 0;

    // Watchdog tracking: rule index -> last seen timestamp_us (0 = not yet seen)
    QHash<int, int64_t> _watchdogLastSeenUs;
    QHash<int, bool> _watchdogFired;
    int64_t _latestTimestampUs = 0;

    QTimer _pollTimer;

    void appendToPreRing(const BusMessage &msg);
    [[nodiscard]] QString evaluateFrameRules(const BusMessage &msg);
    [[nodiscard]] bool matchesCompare(double lhs, TriggerRule::CompareOp op, double rhs) const noexcept;
    void startPostCapture(const QString &reason, double triggerSec, int64_t triggerUs);
    void finalizeCapture();
    [[nodiscard]] QString nextCapturePath() const;
};
