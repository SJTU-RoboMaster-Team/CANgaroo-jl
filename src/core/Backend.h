/*

  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
  Copyright (c) 2026 Schildkroet

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

#include <stdint.h>
#include <atomic>
#include <QObject>
#include <QList>
#include <QMutex>
#include <QDateTime>
#include <QElapsedTimer>
#include "driver/CanDriver.h"
#include "core/BusMessage.h"
#include "db/model/CanDb.h"
#include "core/MeasurementSetup.h"
#include "core/Log.h"

class MeasurementNetwork;
class BusTrace;
class BusListener;
class CanDbMessage;
class SetupDialog;
class LogModel;
class TraceRecorder;
class TriggerCaptureManager;
class MotorManager;

class Backend : public QObject
{
    Q_OBJECT
public:
    static Backend &instance();

    explicit Backend();
    virtual ~Backend();

    void addCanDriver(CanDriver &driver);
    void refreshInterfaces();

    bool startMeasurement(bool preserveTrace = false);
    bool stopMeasurement();
    bool isMeasurementRunning() const;
    bool isPreservingTrace() const;
    bool isReplayRunning() const;
    void setReplayRunning(bool running);
    bool rebindAndResume(QString *errorMessage = nullptr);
    double getTimestampAtMeasurementStart() const;
    uint64_t getUsecsAtMeasurementStart() const;
    uint64_t getNsecsSinceMeasurementStart() const;
    uint64_t getUsecsSinceMeasurementStart() const;


    void logMessage(const QDateTime dt, const log_level_t level, const QString msg);

    MeasurementSetup &getSetup();
    void loadDefaultSetup(MeasurementSetup &setup);
    void setDefaultSetup();
    void setSetup(MeasurementSetup &new_setup);

    double currentTimeStamp() const;

    BusTrace *getTrace();
    void clearTrace();

    TraceRecorder *getTraceRecorder() const { return _traceRecorder; }
    TriggerCaptureManager *getTriggerCaptureManager() const { return _triggerCaptureManager; }
    MotorManager *getMotorManager() const { return _motorManager; }

    /// Built-in database generated from the Motor View configuration. It is
    /// regenerated whenever the motor list changes and is not part of the
    /// measurement setup (so it is never written to a workspace file).
    [[nodiscard]] pCanDb getMotorDb() const;

    CanDbMessage *findDbMessage(const BusMessage &msg) const;

    [[nodiscard]] const QList<CanDriver*> &getDrivers() const noexcept { return _drivers; }
    BusInterfaceIdList getInterfaceList();
    CanDriver *getDriverById(BusInterfaceId id);
    BusInterface *getInterfaceById(BusInterfaceId id);
    QString getInterfaceName(BusInterfaceId id);
    QString getDriverName(BusInterfaceId id);

    CanDriver *getDriverByName(QString driverName);
    BusInterface *getInterfaceByDriverAndName(QString driverName, QString deviceName);

    pCanDb loadDbc(QString filename, QString *errorMsg = nullptr);

    void notifyDisplayConfigChanged();

    void clearLog();
    LogModel &getLogModel() const;

signals:
    void beginMeasurement();
    void endMeasurement();
    void replayStateChanged(bool running);
    void interfaceConnectionLost(const QString &interfaceName, const QString &reason);

    void onSetupChanged();
    void onDisplayConfigChanged();

    void onClearTraceRequested();

    void onLogMessage(const QDateTime dt, const log_level_t level, const QString msg);

    void onSetupDialogCreated(SetupDialog &dlg);

private slots:
    void onListenerDisconnected(BusInterfaceId id, const QString &interfaceName, const QString &reason);
    void onMotorConfigChanged();

private:
    static Backend *_instance;

    std::atomic<bool> _measurementRunning;
    std::atomic<bool> _replayRunning{false};
    bool _preserveTrace{false};
    uint64_t _measurementStartTime;
    QElapsedTimer _timerSinceStart;
    QList<CanDriver*> _drivers;
    MeasurementSetup _setup;
    BusTrace *_trace;
    QList<BusListener*> _listeners;

    LogModel *_logModel;
    TraceRecorder *_traceRecorder = nullptr;
    TriggerCaptureManager *_triggerCaptureManager = nullptr;
    MotorManager *_motorManager = nullptr;

    mutable QMutex _motorDbMutex;
    pCanDb _motorDb;
    // Old databases are kept alive for a while: the trigger evaluator runs on
    // the bus listener thread and may still use a CanDbMessage pointer handed
    // out just before a configuration change. Config changes are UI driven and
    // rare, so a bounded history is enough and prevents a use-after-free.
    QList<pCanDb> _motorDbRetired;
};
