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
#include <memory>

#include <QMutex>
#include <QObject>
#include <QVector>

#include "core/BusMessage.h"
#include "decoders/ProtocolManager.h"
#include "decoders/motor/MotorDecoder.h"
#include "decoders/motor/MotorTypes.h"

class QTimer;

/// Keeps the configured motors up to date. Frames are fed in from the bus
/// listener thread via enqueue(); the decoded state is published to the GUI at
/// a fixed refresh rate from the owning (GUI) thread.
class MotorManager : public QObject
{
    Q_OBJECT

public:
    explicit MotorManager(QObject *parent = nullptr);
    ~MotorManager() override;

    /// Replaces the motor configuration and clears all decoded state.
    void setMotors(const QVector<MotorConfig> &motors);
    [[nodiscard]] QVector<MotorConfig> motors() const;
    [[nodiscard]] QVector<MotorState> states() const;
    [[nodiscard]] bool hasMotors() const;

    /// Thread-safe: called from BusTrace::enqueueMessage() for every frame.
    void enqueue(const BusMessage &msg);

    /// Clears the decoded values but keeps the configuration.
    void resetStates();

    static constexpr qint64 offlineTimeoutMs = 500;

signals:
    void statesUpdated();
    void configChanged();

private slots:
    void onRefreshTimer();

private:
    mutable QMutex _mutex;
    std::shared_ptr<MotorDecoder> _decoder;
    ProtocolManager _protocolManager;
    QVector<MotorConfig> _config;
    QVector<MotorState> _states;
    std::atomic<bool> _dirty{false};
    QTimer *_refreshTimer = nullptr;
};
