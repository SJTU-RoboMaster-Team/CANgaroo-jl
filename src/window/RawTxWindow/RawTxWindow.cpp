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

#include "RawTxWindow.h"
#include "RawTxMessageBuilder.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpressionValidator>
#include <QSplitter>
#include <QDomDocument>
#include <QEvent>
#include <QFont>
#include <QPushButton>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QTimer>

#include "core/Backend.h"
#include "core/Log.h"
#include "core/TraceLineFormat.h"
#include "db/model/CanDbMessage.h"
#include "db/model/CanDbSignal.h"
#include "core/MeasurementSetup.h"
#include "core/MeasurementNetwork.h"
#include "core/MeasurementInterface.h"
#include "driver/BusInterface.h"

namespace {
constexpr int FdEnabledRole = Qt::UserRole + 1;
}

RawTxWindow::RawTxWindow(QWidget *parent, Backend &backend, bool enableSending)
    : ConfigurableWidget(parent)
    , _backend(backend)
    , _currentDbMsg(nullptr)
    , _settingMessage(false)
    , _updatingSignals(false)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);

    // --- Header: ID, DLC, flags ---
    auto *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    _labelId = new QLabel(tr("ID (Hex):"), this);
    headerLayout->addWidget(_labelId);
    _editId = new QLineEdit(this);
    _editId->setMaximumWidth(120);
    _editId->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9A-Fa-f]{0,8}$"), this));
    _editId->setPlaceholderText("000");
    headerLayout->addWidget(_editId);

    _labelDlc = new QLabel(tr("DLC:"), this);
    headerLayout->addWidget(_labelDlc);
    _comboDlc = new QComboBox(this);
    _comboDlc->setMinimumWidth(60);
    headerLayout->addWidget(_comboDlc);

    _cbExtended = new QCheckBox(tr("Extended"), this);
    _cbRTR = new QCheckBox(tr("RTR"), this);
    _cbFD = new QCheckBox(tr("FD"), this);
    _cbBRS = new QCheckBox(tr("BRS"), this);
    _cbBRS->setEnabled(false);

    headerLayout->addWidget(_cbExtended);
    headerLayout->addWidget(_cbRTR);
    headerLayout->addWidget(_cbFD);
    headerLayout->addWidget(_cbBRS);
    headerLayout->addStretch();

    _labelInterface = new QLabel(tr("Interface:"), this);
    headerLayout->addWidget(_labelInterface);
    _comboInterface = new QComboBox(this);
    _comboInterface->setMinimumWidth(180);
    headerLayout->addWidget(_comboInterface);

    mainLayout->addLayout(headerLayout);

    if (enableSending) {
        QSettings settings;
        _lastInterfaceName = settings.value("rawtx/lastInterfaceName").toString();

        auto *sendLayout = new QHBoxLayout();
        sendLayout->setSpacing(8);
        _btnSend = new QPushButton(tr("Send"), this);
        _btnRepeat = new QPushButton(tr("Repeat"), this);
        _btnRepeat->setCheckable(true);
        _labelInterval = new QLabel(tr("Interval"), this);
        _spinRepeat = new QSpinBox(this);
        _spinRepeat->setRange(1, 100000);
        _spinRepeat->setValue(settings.value("rawtx/repeatInterval", 100).toInt());
        _spinRepeat->setSuffix(tr(" ms"));
        _labelInterval->setBuddy(_spinRepeat);
        _sendStatusLabel = new QLabel(this);
        _sendStatusLabel->setTextFormat(Qt::PlainText);
        _sendStatusLabel->setWordWrap(true);
        sendLayout->addWidget(_btnSend);
        sendLayout->addWidget(_btnRepeat);
        sendLayout->addWidget(_labelInterval);
        sendLayout->addWidget(_spinRepeat);
        sendLayout->addWidget(_sendStatusLabel, 1);
        mainLayout->addLayout(sendLayout);

        _repeatTimer = new QTimer(this);
        _repeatTimer->setTimerType(Qt::PreciseTimer);
        _repeatTimer->setInterval(_spinRepeat->value());
        connect(_btnSend, &QPushButton::clicked, this, &RawTxWindow::onSendClicked);
        connect(_btnRepeat, &QPushButton::toggled, this, &RawTxWindow::onRepeatToggled);
        connect(_repeatTimer, &QTimer::timeout, this, &RawTxWindow::onSendClicked);
        connect(_spinRepeat, &QSpinBox::valueChanged, this, [this](int interval) {
            _repeatTimer->setInterval(interval);
            QSettings().setValue("rawtx/repeatInterval", interval);
            updateSendAvailability();
        });
        connect(&backend, &Backend::beginMeasurement, this, [this]() {
            _sendStatus = SendStatus::Idle;
            refreshInterfaces();
        });
        connect(&backend, &Backend::endMeasurement, this, [this]() {
            if (_repeatTimer->isActive()) {
                stopRepeat(SendStatus::MeasurementStopped);
                log_info(tr("Repeat stopped because measurement ended."));
            }
            refreshInterfaces();
        });
        connect(&backend, &Backend::interfaceConnectionLost, this, &RawTxWindow::updateSendAvailability);
    }

    // --- Data hex grid ---
    _dataTable = new QTableWidget(MaxDataRows, DataCols, this);
    _dataTable->horizontalHeader()->setDefaultSectionSize(36);
    _dataTable->verticalHeader()->setDefaultSectionSize(26);
    _dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _dataTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    _dataTable->setSelectionMode(QAbstractItemView::SingleSelection);

    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    _dataTable->setFont(mono);

    // Column headers: byte offsets 0-7
    QStringList colHeaders;
    for (int c = 0; c < DataCols; c++) {
        colHeaders << QString::number(c);
    }
    _dataTable->setHorizontalHeaderLabels(colHeaders);

    // Row headers: offset labels (0, 8, 16, ...)
    QStringList rowHeaders;
    for (int r = 0; r < MaxDataRows; r++) {
        rowHeaders << QString::number(r * DataCols);
    }
    _dataTable->setVerticalHeaderLabels(rowHeaders);

    // Populate cells with "00"
    for (int r = 0; r < MaxDataRows; r++) {
        for (int c = 0; c < DataCols; c++) {
            auto *item = new QTableWidgetItem("00");
            item->setTextAlignment(Qt::AlignCenter);
            _dataTable->setItem(r, c, item);
        }
    }

    // Size the table to fit contents
    int tableHeight = _dataTable->horizontalHeader()->height()
                    + _dataTable->verticalHeader()->defaultSectionSize() + 2;
    _dataTable->setMinimumHeight(tableHeight);
    _dataTable->setMaximumHeight(tableHeight * MaxDataRows);

    // --- Signal table ---
    _signalTable = new QTableWidget(0, 3, this);
    _signalTable->setHorizontalHeaderLabels({tr("Signal"), tr("Value"), tr("Unit")});
    _signalTable->horizontalHeader()->setStretchLastSection(true);
    _signalTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _signalTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);
    _signalTable->setSelectionMode(QAbstractItemView::SingleSelection);

    auto *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(_dataTable);
    splitter->addWidget(_signalTable);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter);

    // --- Populate DLC combo (classic CAN by default) ---
    populateDlcCombo(false);
    _comboDlc->setCurrentIndex(_comboDlc->findData(8));

    // --- Connections ---
    connect(_editId, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text != text.toUpper()) {
            int pos = _editId->cursorPosition();
            _editId->setText(text.toUpper());
            _editId->setCursorPosition(pos);
        }
        // Auto-set extended for IDs > 0x7FF
        if (!_settingMessage) {
            uint32_t id = _editId->text().toUInt(nullptr, 16);
            if (id > 0x7FF || text.length() > 3) {
                _cbExtended->setChecked(true);
            }
        }
        onFieldChanged();
    });

    connect(_comboDlc, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        updateDataGrid();
        onFieldChanged();
    });

    connect(_cbExtended, &QCheckBox::toggled, this, &RawTxWindow::onFieldChanged);
    connect(_cbRTR, &QCheckBox::toggled, this, &RawTxWindow::onFieldChanged);
    connect(_cbFD, &QCheckBox::toggled, this, [this](bool fd) {
        if (!fd && currentDlc() > 8 && !_settingMessage) {
            _cbFD->setChecked(true);
            return;
        }
        _cbBRS->setEnabled(fd);
        _cbRTR->setEnabled(!fd);
        if (fd) {
            _cbRTR->setChecked(false);
        } else {
            _cbBRS->setChecked(false);
        }
        populateDlcCombo(fd);
        updateDataGrid();
        onFieldChanged();
    });
    connect(_cbBRS, &QCheckBox::toggled, this, &RawTxWindow::onFieldChanged);

    connect(_dataTable, &QTableWidget::cellChanged, this, &RawTxWindow::onFieldChanged);
    connect(_signalTable, &QTableWidget::cellChanged, this, &RawTxWindow::onSignalValueChanged);
    connect(_comboInterface, &QComboBox::currentIndexChanged, this, [this](int index) {
        _interfaceId = index >= 0 ? _comboInterface->currentData().value<BusInterfaceId>() : InvalidBusInterfaceId;
        if (index >= 0) {
            _lastInterfaceName = _comboInterface->currentText();
            if (_repeatTimer) {
                QSettings().setValue("rawtx/lastInterfaceName", _lastInterfaceName);
            }
        }
        if (!_settingMessage) {
            if (!_repeatTimer || !_repeatTimer->isActive()) {
                _sendStatus = SendStatus::Idle;
            }
            onFieldChanged();
            updateSendAvailability();
            emit interfaceSelected(_interfaceId);
        }
    });
    connect(&backend, &Backend::onSetupChanged, this, [this]() {
        // The setup can replace the database that owns this message.
        _currentDbMsg = nullptr;
        updateSignalTable();
        refreshInterfaces();
    });

    updateDataGrid();
    refreshInterfaces();
}

void RawTxWindow::retranslateUi()
{
    if (_labelId) _labelId->setText(tr("ID (Hex):"));
    if (_labelDlc) _labelDlc->setText(tr("DLC:"));
    if (_labelInterface) _labelInterface->setText(tr("Interface:"));
    _cbExtended->setText(tr("Extended"));
    _cbRTR->setText(tr("RTR"));
    _cbFD->setText(tr("FD"));
    _cbBRS->setText(tr("BRS"));
    _signalTable->setHorizontalHeaderLabels({tr("Signal"), tr("Value"), tr("Unit")});
    if (_repeatTimer) {
        _btnSend->setText(tr("Send"));
        _btnRepeat->setText(tr("Repeat"));
        _labelInterval->setText(tr("Interval"));
        _spinRepeat->setSuffix(tr(" ms"));
        updateSendAvailability();
    }
}

bool RawTxWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Close && _repeatTimer && _repeatTimer->isActive()) {
        stopRepeat(SendStatus::RepeatStopped);
        updateSendAvailability();
    }
    return ConfigurableWidget::eventFilter(watched, event);
}

int RawTxWindow::currentDlc() const
{
    return _comboDlc->currentIndex() >= 0 ? _comboDlc->currentData().toInt() : -1;
}

void RawTxWindow::populateDlcCombo(bool canfd)
{
    const int prevDlc = currentDlc();
    const QSignalBlocker blocker(_comboDlc);
    _comboDlc->clear();

    for (int d : TraceLineFormat::can_fd_dlc_lengths) {
        if (d > 8 && !canfd) {
            break;
        }
        _comboDlc->addItem(QString::number(d), d);
    }

    // Restore previous DLC if possible
    int idx = _comboDlc->findData(prevDlc);
    if (idx >= 0) {
        _comboDlc->setCurrentIndex(idx);
    }
}

void RawTxWindow::updateDataGrid()
{
    int dlc = currentDlc();
    int rows = (dlc + DataCols - 1) / DataCols;
    if (rows < 1) { rows = 1; }

    // Show/hide rows
    for (int r = 0; r < MaxDataRows; r++) {
        _dataTable->setRowHidden(r, r >= rows);
    }

    // Enable/disable cells beyond DLC
    _dataTable->blockSignals(true);
    for (int r = 0; r < MaxDataRows; r++) {
        for (int c = 0; c < DataCols; c++) {
            int byteIdx = r * DataCols + c;
            QTableWidgetItem *item = _dataTable->item(r, c);
            if (item) {
                if (byteIdx < dlc) {
                    item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsEnabled);
                } else {
                    item->setFlags(item->flags() & ~(Qt::ItemIsEditable | Qt::ItemIsEnabled));
                }
            }
        }
    }
    _dataTable->blockSignals(false);

    // Resize table height to visible rows
    int headerH = _dataTable->horizontalHeader()->height();
    int rowH = _dataTable->verticalHeader()->defaultSectionSize();
    _dataTable->setFixedHeight(headerH + rowH * rows + 2);
}

std::optional<BusMessage> RawTxWindow::buildCurrentMessage(QString *error) const
{
    RawTxMessageInput input;
    input.id = _editId->text().toUInt(nullptr, 16);
    input.extended = _cbExtended->isChecked();
    input.rtr = _cbRTR->isChecked();
    input.fd = _cbFD->isChecked();
    input.brs = _cbBRS->isChecked();
    input.dlc = currentDlc();
    input.interfaceId = _interfaceId;

    if (!input.rtr) {
        for (int i = 0; i < input.dlc; ++i) {
            const auto *item = _dataTable->item(i / DataCols, i % DataCols);
            bool ok = false;
            const uint byte = item ? item->text().toUInt(&ok, 16) : 0;
            if (!ok || byte > 0xFF) {
                if (error) {
                    *error = QStringLiteral(QT_TR_NOOP("Payload bytes must be hexadecimal values from 00 to FF."));
                }
                return std::nullopt;
            }
            input.data[i] = static_cast<quint8>(byte);
        }
    }
    return buildRawTxMessage(input, error);
}

void RawTxWindow::onFieldChanged()
{
    if (_settingMessage) { return; }
    if (const auto message = buildCurrentMessage()) {
        _can_msg = *message;
        updateSignalTable();
        emit messageUpdated(_can_msg);
    }
}

void RawTxWindow::refreshInterfaces()
{
    const BusInterfaceId previousId = _interfaceId;
    const QString previousName = _comboInterface->currentText();
    {
        const QSignalBlocker blocker(_comboInterface);
        _comboInterface->clear();
        for (auto *network : _backend.getSetup().getNetworks()) {
            for (auto *mi : network->interfaces()) {
                const BusInterfaceId id = mi->busInterface();
                if (BusInterface *intf = _backend.getInterfaceById(id)) {
                    const QString name = network->name() + ": " + intf->getName();
                    _comboInterface->addItem(name, QVariant::fromValue(id));
                    const bool fdEnabled = mi->isCanFD() && (intf->getCapabilities() & BusInterface::capability_canfd);
                    _comboInterface->setItemData(_comboInterface->count() - 1, fdEnabled, FdEnabledRole);
                }
            }
        }
        const QString preferredName = previousName.isEmpty() ? _lastInterfaceName : previousName;
        int index = _comboInterface->findText(preferredName);
        if (index < 0 && _comboInterface->count() > 0) {
            index = 0;
        }
        _comboInterface->setCurrentIndex(index);
        _interfaceId = index >= 0 ? _comboInterface->currentData().value<BusInterfaceId>() : InvalidBusInterfaceId;
    }
    if (_repeatTimer) {
        setEnabled(_comboInterface->count() > 0);
        if (_repeatTimer->isActive()
            && (_interfaceId != previousId || _comboInterface->currentText() != previousName)) {
            reportSendFailure(QStringLiteral(QT_TR_NOOP("The selected interface changed during Repeat.")));
        }
    }
    onFieldChanged();
    updateSendAvailability();
}

void RawTxWindow::updateSendAvailability()
{
    if (!_repeatTimer) { return; }

    BusInterface *intf = _backend.getInterfaceById(_interfaceId);
    const bool ready = _backend.isMeasurementRunning() && intf && intf->isOpen();
    if (!ready && _repeatTimer->isActive()) {
        reportSendFailure(QStringLiteral(QT_TR_NOOP("The selected interface is not open.")));
    }

    const bool repeating = _repeatTimer->isActive();
    _btnSend->setEnabled(ready && !repeating);
    _btnRepeat->setEnabled(ready);

    QString status;
    if (!ready && (_sendStatus == SendStatus::Idle || _sendStatus == SendStatus::Sent
                   || _sendStatus == SendStatus::RepeatStopped)) {
        _sendStatusLabel->setText(tr("Interface is closed. Start measurement and select an open interface."));
        return;
    }
    switch (_sendStatus) {
    case SendStatus::Idle:
        status = tr("Ready to send");
        break;
    case SendStatus::Sent:
        status = repeating ? tr("TX requested: %1 (every %2 ms)").arg(_sentCount).arg(_repeatTimer->interval())
                           : tr("TX requested: %1").arg(_sentCount);
        break;
    case SendStatus::RepeatStopped:
        status = tr("Repeat stopped. TX requested: %1").arg(_sentCount);
        break;
    case SendStatus::MeasurementStopped:
        status = tr("Repeat stopped because measurement ended.");
        break;
    case SendStatus::Failed:
        status = tr("Send failed: %1").arg(tr(qPrintable(_lastSendError)));
        break;
    }
    _sendStatusLabel->setText(status);
}

void RawTxWindow::stopRepeat(SendStatus status)
{
    _repeatTimer->stop();
    const QSignalBlocker blocker(_btnRepeat);
    _btnRepeat->setChecked(false);
    _sendStatus = status;
}

void RawTxWindow::reportSendFailure(const QString &error)
{
    _lastSendError = error;
    stopRepeat(SendStatus::Failed);
    log_error(tr("Send failed: %1").arg(tr(qPrintable(error)))
              + QStringLiteral(" [%1]").arg(_comboInterface->currentText()));
}

void RawTxWindow::onSendClicked()
{
    QString error;
    if (sendCurrentMessage(&error)) {
        ++_sentCount;
        _sendStatus = SendStatus::Sent;
        if (_btnRepeat->isChecked() && !_repeatTimer->isActive()) {
            _repeatTimer->start();
        }
    } else {
        reportSendFailure(error);
    }
    updateSendAvailability();
}

void RawTxWindow::onRepeatToggled(bool checked)
{
    if (checked) {
        onSendClicked();
    } else {
        stopRepeat(SendStatus::RepeatStopped);
        updateSendAvailability();
    }
}

bool RawTxWindow::sendCurrentMessage(QString *error)
{
    const BusInterfaceId id = _comboInterface->currentIndex() >= 0
        ? _comboInterface->currentData().value<BusInterfaceId>() : InvalidBusInterfaceId;
    BusInterface *intf = _backend.getInterfaceById(id);
    if (!_backend.isMeasurementRunning() || !intf || !intf->isOpen()) {
        *error = QStringLiteral(QT_TR_NOOP("The selected interface is not open."));
        return false;
    }

    const auto message = buildCurrentMessage(error);
    if (!message) {
        return false;
    }
    if (message->isFD() && !_comboInterface->currentData(FdEnabledRole).toBool()) {
        *error = QStringLiteral(QT_TR_NOOP("CAN FD is not enabled on the selected interface."));
        return false;
    }
    intf->sendMessage(*message);
    return true;
}

void RawTxWindow::updateSignalTable()
{
    _signalTable->blockSignals(true);

    if (!_currentDbMsg) {
        _signalTable->setRowCount(0);
        _signalTable->blockSignals(false);
        return;
    }

    CanDbSignalList sigList = _currentDbMsg->getSignals();

    if (_signalTable->rowCount() != sigList.size()) {
        // Full rebuild when the signal structure changes (new message selected)
        _signalTable->setRowCount(0);
        _signalTable->setRowCount(sigList.size());

        for (int i = 0; i < sigList.size(); i++) {
            CanDbSignal *sig = sigList[i];

            auto *nameItem = new QTableWidgetItem(sig->name());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            nameItem->setToolTip(sig->startBitDescription());
            _signalTable->setItem(i, 0, nameItem);

            _signalTable->setItem(i, 1, new QTableWidgetItem(QString::number(sig->extractPhysicalFromMessage(_can_msg), 'f', 2)));

            auto *unitItem = new QTableWidgetItem(sig->getUnit());
            unitItem->setFlags(unitItem->flags() & ~Qt::ItemIsEditable);
            _signalTable->setItem(i, 2, unitItem);
        }
    } else {
        // Update values in-place — avoids destroying the active editor item
        for (int i = 0; i < sigList.size(); i++) {
            double val = sigList[i]->extractPhysicalFromMessage(_can_msg);
            if (QTableWidgetItem *item = _signalTable->item(i, 1)) {
                item->setText(QString::number(val, 'f', 2));
            }
        }
    }

    _signalTable->blockSignals(false);
}

void RawTxWindow::onSignalValueChanged(int row, int col)
{
    if (col != 1 || !_currentDbMsg || _settingMessage || _updatingSignals) { return; }

    CanDbSignalList sigList = _currentDbMsg->getSignals();
    if (row < 0 || row >= sigList.size()) { return; }

    QTableWidgetItem *item = _signalTable->item(row, col);
    if (!item) { return; }

    bool ok;
    double value = item->text().toDouble(&ok);
    if (!ok) { return; }

    _updatingSignals = true;

    sigList[row]->injectPhysicalIntoMessage(_can_msg, value);

    _dataTable->blockSignals(true);
    int dlc = _can_msg.getLength();
    for (int i = 0; i < dlc && i < 64; i++) {
        int r = i / DataCols;
        int c = i % DataCols;
        QTableWidgetItem *dataItem = _dataTable->item(r, c);
        if (dataItem) {
            dataItem->setText(QString("%1").arg(_can_msg.getByte(i), 2, 16, QChar('0')).toUpper());
        }
    }
    _dataTable->blockSignals(false);

    updateSignalTable();
    emit messageUpdated(_can_msg);

    _updatingSignals = false;
}

void RawTxWindow::setMessage(const BusMessage &msg, const QString &name, BusInterfaceId interfaceId, CanDbMessage *dbMsg)
{
    _settingMessage = true;
    Q_UNUSED(name);

    if (_currentDbMsg != dbMsg) {
        const QSignalBlocker blocker(_signalTable);
        _signalTable->setRowCount(0);
    }
    _currentDbMsg = dbMsg;

    const int interfaceIndex = _comboInterface->findData(QVariant::fromValue(interfaceId));
    _comboInterface->setCurrentIndex(interfaceIndex);
    _interfaceId = interfaceIndex >= 0 || !_repeatTimer ? interfaceId : InvalidBusInterfaceId;
    const int length = msg.getLength();
    const bool fd = msg.isFD() || length > 8;

    _editId->setText(QString("%1").arg(msg.getId(), 0, 16).toUpper());
    _cbExtended->setChecked(msg.isExtended());
    _cbFD->setChecked(fd);
    _cbRTR->setChecked(msg.isRTR() && !fd);
    _cbBRS->setChecked(msg.isBRS() && fd);
    _comboDlc->setCurrentIndex(_comboDlc->findData(length));

    // Load data bytes into grid
    _dataTable->blockSignals(true);
    for (int i = 0; i < 64; i++) {
        int r = i / DataCols;
        int c = i % DataCols;
        QTableWidgetItem *item = _dataTable->item(r, c);
        if (item) {
            item->setText(QString("%1").arg(msg.getByte(i), 2, 16, QChar('0')).toUpper());
        }
    }
    _dataTable->blockSignals(false);

    updateDataGrid();

    _settingMessage = false;
    onFieldChanged(); // populates _can_msg from the current UI state
    updateSendAvailability();
}

bool RawTxWindow::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    if (!ConfigurableWidget::saveXML(backend, xml, root)) { return false; }
    root.setAttribute("type", "RawTxWindow");
    return true;
}

bool RawTxWindow::loadXML(Backend &backend, QDomElement &el)
{
    if (!ConfigurableWidget::loadXML(backend, el)) { return false; }
    return true;
}
