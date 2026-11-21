/*
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

#include <optional>

#include "core/Backend.h"
#include "core/ConfigurableWidget.h"
#include "core/MeasurementSetup.h"

class QLineEdit;
class QLabel;
class QComboBox;
class QCheckBox;
class QTableWidget;
class QPushButton;
class QSpinBox;
class QTimer;
class QDomDocument;
class QDomElement;
class CanDbMessage;

class RawTxWindow : public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit RawTxWindow(QWidget *parent, Backend &backend, bool enableSending = true);

    bool saveXML(Backend &backend, QDomDocument &xml, QDomElement &root) override;
    bool loadXML(Backend &backend, QDomElement &el) override;

protected:
    void retranslateUi() override;
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void setMessage(const BusMessage &msg, const QString &name, BusInterfaceId interfaceId, CanDbMessage *dbMsg = nullptr);

signals:
    void messageUpdated(const BusMessage &msg);
    void interfaceSelected(BusInterfaceId interfaceId);

private slots:
    void onFieldChanged();
    void onSignalValueChanged(int row, int col);

private:
    enum class SendStatus { Idle, Sent, RepeatStopped, MeasurementStopped, Failed };

    Backend &_backend;
    BusMessage _can_msg;
    CanDbMessage *_currentDbMsg;
    BusInterfaceId _interfaceId = InvalidBusInterfaceId;
    bool _settingMessage;
    bool _updatingSignals;

    QLabel *_labelId = nullptr;
    QLabel *_labelDlc = nullptr;
    QLabel *_labelInterface = nullptr;
    QLineEdit *_editId;
    QComboBox *_comboDlc;
    QComboBox *_comboInterface;
    QCheckBox *_cbExtended;
    QCheckBox *_cbRTR;
    QCheckBox *_cbFD;
    QCheckBox *_cbBRS;
    QTableWidget *_dataTable;
    QTableWidget *_signalTable;

    QLabel *_labelInterval = nullptr;
    QPushButton *_btnSend = nullptr;
    QPushButton *_btnRepeat = nullptr;
    QSpinBox *_spinRepeat = nullptr;
    QLabel *_sendStatusLabel = nullptr;
    QTimer *_repeatTimer = nullptr;
    quint64 _sentCount = 0;
    SendStatus _sendStatus = SendStatus::Idle;
    QString _lastSendError;
    QString _lastInterfaceName;

    static constexpr int DataCols = 8;
    static constexpr int MaxDataRows = 8; // 8 rows × 8 cols = 64 bytes

    void updateDataGrid();
    void updateSignalTable();
    void populateDlcCombo(bool canfd);
    [[nodiscard]] int currentDlc() const;
    [[nodiscard]] std::optional<BusMessage> buildCurrentMessage(QString *error = nullptr) const;
    void refreshInterfaces();
    void updateSendAvailability();
    void onSendClicked();
    void onRepeatToggled(bool checked);
    void stopRepeat(SendStatus status);
    void reportSendFailure(const QString &error);
    [[nodiscard]] bool sendCurrentMessage(QString *error);
};
