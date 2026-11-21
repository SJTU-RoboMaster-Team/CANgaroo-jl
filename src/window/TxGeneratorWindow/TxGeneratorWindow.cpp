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

#include "TxGeneratorWindow.h"
#include "ui_TxGeneratorWindow.h"

#include <algorithm>
#include <chrono>
#include <optional>

#include <QTreeWidgetItem>
#include <QAction>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSignalBlocker>
#include "core/Backend.h"
#include "core/HexData.h"
#include "core/ThemeManager.h"
#include "core/MeasurementNetwork.h"
#include "core/MeasurementSetup.h"
#include "core/MeasurementInterface.h"
#include "driver/BusInterface.h"
#include "driver/CanDriver.h"
#include "window/RawTxWindow/RawTxWindow.h"
#include <chrono>

namespace
{
/// CAN / CAN-FD DLC values that a frame may legally use.
bool isCanFdPayloadLength(int length)
{
    if (length < 0 || length > BusMessage::k_maxDataBytes)
        return false;
    if (length <= 8)
        return true;
    return length == 12 || length == 16 || length == 20 || length == 24
        || length == 32 || length == 48 || length == 64;
}
} // namespace

TxGeneratorWindow::TxGeneratorWindow(QWidget *parent, Backend &backend) :
    ConfigurableWidget(parent),
    ui(new Ui::TxGeneratorWindow),
    _backend(backend)
{
    ui->setupUi(this);

    // Allow tree widgets to be compressed to near-zero so the window can be
    // made as short as the user wants (otherwise QTreeWidget's inherent
    // minimumSizeHint blocks shrinkage even after the window minimum is relaxed).
    ui->treeAvailable->setMinimumHeight(0);
    ui->treeActive->setMinimumHeight(0);

    // Deadline-driven scheduler: instead of polling, the timer is re-armed after
    // every pass for the earliest pending send deadline. That wakes the event loop
    // once per due frame (10x/s at a 100 ms cycle instead of 250x/s) and removes
    // the tick quantization that used to jitter the cycle time by up to one tick.
    // PreciseTimer is mandatory here -- the default CoarseTimerType grants itself
    // 5% slop, which would be +-5 ms at a 100 ms interval.
    _sendTimer = new QTimer(this);
    _sendTimer->setSingleShot(true);
    _sendTimer->setTimerType(Qt::PreciseTimer);
    connect(_sendTimer, &QTimer::timeout, this, &TxGeneratorWindow::onSendTimerTimeout);

    connect(&backend, &Backend::onSetupChanged, this, &TxGeneratorWindow::onSetupChanged);
    connect(&backend, &Backend::beginMeasurement, this, &TxGeneratorWindow::refreshInterfaces);
    connect(&backend, &Backend::beginMeasurement, this, &TxGeneratorWindow::updateMeasurementState);
    connect(&backend, &Backend::endMeasurement, this, &TxGeneratorWindow::refreshInterfaces);
    connect(&backend, &Backend::endMeasurement, this, &TxGeneratorWindow::updateMeasurementState);

    connect(ui->btnBulkRun, &QPushButton::clicked, this, &TxGeneratorWindow::on_btnBulkRun_clicked);
    connect(ui->btnBulkStop, &QPushButton::clicked, this, &TxGeneratorWindow::on_btnBulkStop_clicked);

    // Semantic button roles are styled by the global theme stylesheet.
    ui->btnBulkRun->setProperty("uiRole", QStringLiteral("primary"));
    ui->btnBulkStop->setProperty("uiRole", QStringLiteral("danger"));

    connect(ui->treeActive, &QTreeWidget::itemChanged, this, &TxGeneratorWindow::on_treeActive_itemChanged);
    connect(ui->treeActive, &QTreeWidget::itemDoubleClicked, this, &TxGeneratorWindow::treeActiveItemDoubleClicked);

    connect(ui->treeAvailable, &QTreeWidget::itemSelectionChanged, this, &TxGeneratorWindow::on_treeAvailable_itemSelectionChanged);

    // Manual entry: Enter in either field adds the message, and a valid payload
    // keeps the DLC spin box in sync while typing.
    connect(ui->lineManualId, &QLineEdit::returnPressed, this, [this]() { addManualMessage(); });
    connect(ui->lineManualData, &QLineEdit::returnPressed, this, [this]() { addManualMessage(); });

    // Hint label under the manual entry; auto-clears so errors do not pile up.
    _hintTimer = new QTimer(this);
    _hintTimer->setSingleShot(true);
    _hintTimer->setInterval(6000);
    connect(_hintTimer, &QTimer::timeout, this, &TxGeneratorWindow::clearHint);

    // Context-menu actions for the active list. They are children of this
    // window so they survive menu instances and can be triggered by tests.
    _actionEditMessage = new QAction(tr("Edit..."), this);
    _actionEditMessage->setObjectName(QStringLiteral("actionGeneratorEdit"));
    connect(_actionEditMessage, &QAction::triggered, this, [this]() {
        const QList<int> rows = selectedRows();
        if (rows.size() == 1) { openMessageEditor(rows.first()); }
    });
    _actionSendOnce = new QAction(tr("Send Once"), this);
    _actionSendOnce->setObjectName(QStringLiteral("actionGeneratorSendOnce"));
    connect(_actionSendOnce, &QAction::triggered, this, [this]() { sendOnceRows(selectedRows()); });
    _actionRun = new QAction(tr("Run"), this);
    _actionRun->setObjectName(QStringLiteral("actionGeneratorRun"));
    connect(_actionRun, &QAction::triggered, this, [this]() { setRowsEnabled(selectedRows(), true); });
    _actionStop = new QAction(tr("Stop"), this);
    _actionStop->setObjectName(QStringLiteral("actionGeneratorStop"));
    connect(_actionStop, &QAction::triggered, this, [this]() { setRowsEnabled(selectedRows(), false); });
    _actionDuplicate = new QAction(tr("Duplicate"), this);
    _actionDuplicate->setObjectName(QStringLiteral("actionGeneratorDuplicate"));
    connect(_actionDuplicate, &QAction::triggered, this, [this]() { duplicateRows(selectedRows()); });
    _actionRename = new QAction(tr("Rename..."), this);
    _actionRename->setObjectName(QStringLiteral("actionGeneratorRename"));
    connect(_actionRename, &QAction::triggered, this, [this]() {
        const QList<int> rows = selectedRows();
        if (rows.size() == 1) { renameRow(rows.first()); }
    });
    _actionRemove = new QAction(tr("Remove"), this);
    _actionRemove->setObjectName(QStringLiteral("actionGeneratorRemove"));
    connect(_actionRemove, &QAction::triggered, this, [this]() { removeRows(selectedRows()); });

    connect(ui->treeActive, &QWidget::customContextMenuRequested,
            this, &TxGeneratorWindow::showActiveContextMenu);

    _bitMatrixWidget = new BitMatrixWidget(this);
    // REMOVED redundant addWidget to verticalLayoutTabLayout

    // Configure Scroll Area
    ui->scrollAreaLayout->setWidgetResizable(false);
    ui->scrollAreaLayout->setWidget(_bitMatrixWidget);

    // Initialize Layout View controls
    ui->sliderLayoutZoom->setRange(30, 120);
    ui->sliderLayoutZoom->setValue(50);
    _bitMatrixWidget->setCellSize(50);
    _bitMatrixWidget->setFixedSize(_bitMatrixWidget->sizeHint());

    ui->spinInterval->setSuffix(tr(" ms"));
    ui->spinInterval->setMinimumWidth(0);

    ui->lineManualId->setInputMask("");
    ui->lineManualId->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9A-Fa-f]{0,8}$"), this));
    ui->lineManualId->setText("001");
    connect(ui->lineManualId, &QLineEdit::textChanged, this, [this](const QString &text){
        if (text != text.toUpper()) {
            int cursorPos = ui->lineManualId->cursorPosition();
            ui->lineManualId->setText(text.toUpper());
            ui->lineManualId->setCursorPosition(cursorPos);
        }
    });

    ui->treeActive->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeAvailable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->btnAddToList->setEnabled(false);
    ui->treeActive->setToolTip(tr("Right-click a row to edit, send once, duplicate, rename or remove it."));

    // The active list is the main work area; give it a bit more room than the
    // DBC picker when the dock is resized.
    ui->splitter->setStretchFactor(0, 4);
    ui->splitter->setStretchFactor(1, 6);
    ui->splitter->setSizes({400, 700});

    // Fit columns to content; Name and Data share the remaining width.
    ui->treeActive->header()->setStretchLastSection(false);
    ui->treeActive->header()->setSectionResizeMode(colStatus, QHeaderView::Fixed);
    ui->treeActive->header()->resizeSection(colStatus, 48);
    ui->treeActive->header()->setSectionResizeMode(colId, QHeaderView::ResizeToContents);
    ui->treeActive->header()->setSectionResizeMode(colName, QHeaderView::Stretch);
    ui->treeActive->header()->setSectionResizeMode(colData, QHeaderView::Stretch);
    ui->treeActive->header()->setSectionResizeMode(colInterface, QHeaderView::ResizeToContents);
    ui->treeActive->header()->setSectionResizeMode(colDlc, QHeaderView::ResizeToContents);
    ui->treeActive->header()->setSectionResizeMode(colInterval, QHeaderView::ResizeToContents);

    refreshInterfaces();
    updateMeasurementState();
    populateDbcMessages();
    updateActiveList();
    isLoading = false;
}

TxGeneratorWindow::~TxGeneratorWindow()
{
    delete ui;
}

void TxGeneratorWindow::retranslateUi()
{
    ui->retranslateUi(this);

    ui->spinInterval->setSuffix(tr(" ms"));

    if (_actionEditMessage) { _actionEditMessage->setText(tr("Edit...")); }
    if (_actionSendOnce) { _actionSendOnce->setText(tr("Send Once")); }
    if (_actionRun) { _actionRun->setText(tr("Run")); }
    if (_actionStop) { _actionStop->setText(tr("Stop")); }
    if (_actionDuplicate) { _actionDuplicate->setText(tr("Duplicate")); }
    if (_actionRename) { _actionRename->setText(tr("Rename...")); }
    if (_actionRemove) { _actionRemove->setText(tr("Remove")); }

    // Status button tooltips and the interface column are set per row in code.
    updateActiveList();
    updateRunStatus();
}

bool TxGeneratorWindow::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    if (!ConfigurableWidget::saveXML(backend, xml, root)) { return false; }
    root.setAttribute("type", "TxGeneratorWindow");
    root.setAttribute("splitterState", QString(ui->splitter->saveState().toBase64()));

    for (const CyclicMessage &cm : _cyclicMessages)
    {
        // Resolve the display name of the selected interface from the combo box.
        QString ifName;
        for (int i = 0; i < ui->comboBoxInterface->count(); i++)
        {
            if (static_cast<BusInterfaceId>(ui->comboBoxInterface->itemData(i).toUInt()) == cm.interfaceId)
            {
                ifName = ui->comboBoxInterface->itemText(i);
                break;
            }
        }

        // Encode payload as space-separated uppercase hex bytes.
        QString dataStr;
        for (int i = 0; i < cm.msg.getLength(); i++)
        {
            if (i > 0) { dataStr += ' '; }
            dataStr += QString("%1").arg(cm.msg.getByte(i), 2, 16, QChar('0')).toUpper();
        }

        QDomElement frameEl = xml.createElement("frame");
        frameEl.setAttribute("id",       QString("0x%1").arg(cm.msg.getId(), 0, 16).toUpper());
        frameEl.setAttribute("name",     cm.name);
        frameEl.setAttribute("dlc",      cm.msg.getLength());
        frameEl.setAttribute("extended", cm.msg.isExtended() ? 1 : 0);
        frameEl.setAttribute("fd",       cm.msg.isFD()       ? 1 : 0);
        frameEl.setAttribute("brs",      cm.msg.isBRS()      ? 1 : 0);
        frameEl.setAttribute("interval", cm.interval);
        frameEl.setAttribute("interface", ifName);
        frameEl.setAttribute("data",     dataStr);
        root.appendChild(frameEl);
    }

    return true;
}

bool TxGeneratorWindow::loadXML(Backend &backend, QDomElement &el)
{
    if (!ConfigurableWidget::loadXML(backend, el)) { return false; }

    QString splitterState = el.attribute("splitterState");
    if (!splitterState.isEmpty()) {
        ui->splitter->restoreState(QByteArray::fromBase64(splitterState.toLatin1()));
    }

    _cyclicMessages.clear();

    QDomNodeList frames = el.elementsByTagName("frame");
    for (int i = 0; i < frames.count(); i++)
    {
        QDomElement frameEl = frames.at(i).toElement();

        CyclicMessage cm;
        cm.msg.setId(frameEl.attribute("id").toUInt(nullptr, 16));
        const int dlc = frameEl.attribute("dlc").toInt();
        cm.msg.setLength(static_cast<uint8_t>(dlc));
        cm.msg.setExtended(frameEl.attribute("extended").toInt() != 0);
        cm.msg.setFD(      frameEl.attribute("fd").toInt()       != 0);
        cm.msg.setBRS(     frameEl.attribute("brs").toInt()      != 0);
        cm.name          = frameEl.attribute("name");
        cm.interval      = frameEl.attribute("interval", "100").toInt();
        cm.enabled       = false; // Never auto-start on load
        cm.nextDue       = {};
        cm.interfaceId   = 0;
        cm.dbMsg         = nullptr;
        cm.interfaceName = frameEl.attribute("interface");

        // Decode space-separated hex payload.
        const QStringList bytes = frameEl.attribute("data").split(' ', Qt::SkipEmptyParts);
        for (int j = 0; j < bytes.size() && j < dlc; j++)
        {
            cm.msg.setDataAt(static_cast<uint8_t>(j),
                             static_cast<uint8_t>(bytes[j].toUInt(nullptr, 16)));
        }

        _cyclicMessages.append(cm);
    }

    // Interface IDs are runtime-assigned; try to resolve them now (may be a
    // no-op if the setup hasn't been loaded yet — resolveInterfaceNames() is
    // also called from refreshInterfaces() once the setup is applied).
    resolveInterfaceNames();
    // dbMsg is a runtime pointer and can't be persisted; re-link by raw CAN ID
    // now (may be a no-op if the setup/DBCs haven't been loaded yet —
    // resolveDbMessages() is also called from refreshInterfaces()).
    resolveDbMessages();
    updateActiveList();
    return true;
}

void TxGeneratorWindow::resolveInterfaceNames()
{
    for (CyclicMessage &cm : _cyclicMessages)
    {
        if (cm.interfaceName.isEmpty()) { continue; }
        for (int i = 0; i < ui->comboBoxInterface->count(); i++)
        {
            if (ui->comboBoxInterface->itemText(i) == cm.interfaceName)
            {
                cm.interfaceId = static_cast<BusInterfaceId>(ui->comboBoxInterface->itemData(i).toUInt());
                break;
            }
        }
    }
}

void TxGeneratorWindow::resolveDbMessages()
{
    MeasurementSetup &setup = _backend.getSetup();
    for (CyclicMessage &cm : _cyclicMessages)
    {
        if (cm.dbMsg) { continue; }
        cm.dbMsg = setup.findDbMessage(cm.msg);
    }
}

void TxGeneratorWindow::refreshInterfaces()
{
    ui->comboBoxInterface->blockSignals(true);
    ui->comboBoxInterface->clear();

    MeasurementSetup &setup = _backend.getSetup();
    for (auto *network : setup.getNetworks()) {
        for (auto *mi : network->interfaces()) {
            BusInterfaceId ifid = mi->busInterface();
            BusInterface *intf = _backend.getInterfaceById(ifid);
            if (intf) {
                QString name = network->name() + ": " + intf->getName();
                ui->comboBoxInterface->addItem(name, QVariant(ifid));
            }
        }
    }
    if (ui->comboBoxInterface->count() > 0 && ui->comboBoxInterface->currentIndex() == -1) {
        ui->comboBoxInterface->setCurrentIndex(0);
    }
    ui->comboBoxInterface->blockSignals(false);

    // Re-resolve interface names in case frames were loaded before the setup existed.
    resolveInterfaceNames();
    // Re-resolve dbMsg links in case frames were loaded before the DBCs existed.
    resolveDbMessages();
    populateDbcMessages();
}

void TxGeneratorWindow::populateDbcMessages()
{
    ui->treeAvailable->clear();

    BusInterfaceId currentId = static_cast<BusInterfaceId>(ui->comboBoxInterface->currentData().toUInt());
    MeasurementSetup &setup = _backend.getSetup();

    for (auto *network : setup.getNetworks()) {
        // Only show DBCs associated with the current interface if possible,
        // but currently networks map to interfaces.
        // Let's find if this network is using our interface.
        bool interfaceMatches = false;
        for (auto *mi : network->interfaces()) {
            if (mi->busInterface() == currentId) {
                interfaceMatches = true;
                break;
            }
        }

        if (interfaceMatches) {
            for (const auto &db : network->_canDbs) {
                if (db) {
                    CanDbMessageList msgs = db->getMessageList();
                    for (auto it = msgs.begin(); it != msgs.end(); ++it) {
                        CanDbMessage *dbMsg = *it;
                        if (dbMsg) {
                            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeAvailable);
                            item->setText(0, "0x" + QString("%1").arg(dbMsg->getRaw_id(), 3, 16, QChar('0')).toUpper());
                            item->setText(1, dbMsg->getName());
                            item->setData(0, Qt::UserRole, QVariant::fromValue((void*)dbMsg));
                        }
                    }
                }
            }
        }
    }
}

void TxGeneratorWindow::on_lineEditSearchAvailable_textChanged(const QString &text)
{
    for (int i = 0; i < ui->treeAvailable->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeAvailable->topLevelItem(i);
        bool match = item->text(0).contains(text, Qt::CaseInsensitive) || item->text(1).contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

void TxGeneratorWindow::on_treeAvailable_itemSelectionChanged()
{
    ui->btnAddToList->setEnabled(!ui->treeAvailable->selectedItems().isEmpty());

    QTreeWidgetItem *item = ui->treeAvailable->currentItem();
    if (item && _bitMatrixWidget) {
        CanDbMessage *dbMsg = static_cast<CanDbMessage*>(item->data(0, Qt::UserRole).value<void*>());
        _bitMatrixWidget->setMessage(dbMsg);
    } else if (_bitMatrixWidget) {
        _bitMatrixWidget->setMessage(nullptr);
    }
}

void TxGeneratorWindow::on_sliderLayoutZoom_valueChanged(int value)
{
    if (_bitMatrixWidget) {
        _bitMatrixWidget->setCellSize(value);
        _bitMatrixWidget->setFixedSize(_bitMatrixWidget->sizeHint());
    }
}

void TxGeneratorWindow::on_cbLayoutCompact_toggled(bool checked)
{
    if (_bitMatrixWidget) {
        _bitMatrixWidget->setCompactMode(checked);
        _bitMatrixWidget->setFixedSize(_bitMatrixWidget->sizeHint());
    }
}

void TxGeneratorWindow::on_btnAddToList_released()
{
    QList<QTreeWidgetItem *> selected = ui->treeAvailable->selectedItems();
    if (selected.isEmpty()) {
        QTreeWidgetItem *current = ui->treeAvailable->currentItem();
        if (current) { selected.append(current); }
    }

    if (selected.isEmpty()) {
        setHint(tr("Select a DBC message in the list first."));
        return;
    }

    addAvailableMessages(selected);
}

void TxGeneratorWindow::addAvailableMessages(const QList<QTreeWidgetItem *> &items)
{
    const int firstNew = _cyclicMessages.size();
    int added = 0;

    for (auto *item : items) {
        if (!item) { continue; }
        CanDbMessage *dbMsg = static_cast<CanDbMessage *>(item->data(0, Qt::UserRole).value<void *>());
        if (!dbMsg) { continue; }

        CyclicMessage cm;
        cm.msg = BusMessage(); // Ensure fresh instance
        cm.msg.setId(dbMsg->getRaw_id());
        cm.msg.setLength(dbMsg->getDlc());
        cm.msg.setExtended(dbMsg->getRaw_id() > 0x7FF);
        cm.name = dbMsg->getName();
        // Use the interval the user configured in the Generator View instead of
        // silently hardcoding it. A row added from the DBC list can still be
        // changed with the interval spin box afterwards.
        cm.interval = ui->spinInterval->value();
        cm.enabled = false;
        cm.nextDue = {};
        cm.interfaceId = static_cast<BusInterfaceId>(ui->comboBoxInterface->currentData().toUInt());
        cm.dbMsg = dbMsg;

        _cyclicMessages.append(cm);
        ++added;
    }

    if (added == 0) {
        setHint(tr("No DBC message selected."));
        return;
    }

    updateActiveList();
    ui->treeActive->clearSelection();
    for (int row = firstNew; row < _cyclicMessages.size(); ++row) {
        if (QTreeWidgetItem *newItem = ui->treeActive->topLevelItem(row)) {
            newItem->setSelected(true);
        }
    }
    if (QTreeWidgetItem *first = ui->treeActive->topLevelItem(firstNew)) {
        ui->treeActive->setCurrentItem(first);
    }
    ui->treeActive->scrollToBottom();
    setHint(tr("Added %1 message(s).").arg(added));
}

void TxGeneratorWindow::on_btnAddManual_released()
{
    addManualMessage();
}

void TxGeneratorWindow::addManualMessage()
{
    bool ok = false;
    const uint32_t id = ui->lineManualId->text().toUInt(&ok, 16);
    if (!ok) {
        setHint(tr("Manual message: enter a hexadecimal CAN ID first."));
        return;
    }

    const QString dataText = ui->lineManualData->text().trimmed();
    QByteArray payload;
    if (dataText.isEmpty()) {
        const int dlc = ui->spinManualDlc->value();
        if (!isCanFdPayloadLength(dlc)) {
            setHint(tr("Manual message: CAN FD DLC must be 0-8, 12, 16, 20, 24, 32, 48 or 64 bytes."));
            return;
        }
        payload = QByteArray(dlc, '\0');
    } else {
        const auto parsed = HexData::parseByteString(dataText);
        if (!parsed) {
            setHint(tr("Manual message: \"%1\" is not a valid hex byte string (e.g. 00 11 22).").arg(dataText));
            return;
        }
        payload = *parsed;
        if (payload.size() > BusMessage::k_maxDataBytes) {
            setHint(tr("Manual message: at most %1 data bytes are supported.")
                        .arg(BusMessage::k_maxDataBytes));
            return;
        }
        if (!isCanFdPayloadLength(payload.size())) {
            setHint(tr("Manual message: payload length %1 is not a valid CAN FD DLC "
                       "(12, 16, 20, 24, 32, 48 or 64).").arg(payload.size()));
            return;
        }
    }

    CyclicMessage cm;
    cm.msg = BusMessage(); // Ensure fresh instance
    cm.msg.setId(id);
    cm.msg.setLength(static_cast<uint8_t>(payload.size()));
    cm.msg.setExtended(id > 0x7FF || ui->lineManualId->text().length() > 3);
    cm.msg.setFD(payload.size() > 8);
    for (int i = 0; i < payload.size(); ++i) {
        cm.msg.setDataAt(static_cast<uint8_t>(i), static_cast<uint8_t>(payload.at(i)));
    }
    cm.name = tr("Manual 0x%1").arg(QString::number(id, 16).toUpper());
    cm.interval = ui->spinInterval->value();
    cm.enabled = false;
    cm.nextDue = {};
    cm.interfaceId = static_cast<BusInterfaceId>(ui->comboBoxInterface->currentData().toUInt());
    cm.dbMsg = nullptr;

    _cyclicMessages.append(cm);

    const QSignalBlocker blockDlc(ui->spinManualDlc);
    ui->spinManualDlc->setValue(payload.size());

    updateActiveList();
    ui->treeActive->clearSelection();
    if (QTreeWidgetItem *newItem = ui->treeActive->topLevelItem(_cyclicMessages.size() - 1)) {
        newItem->setSelected(true);
        ui->treeActive->setCurrentItem(newItem);
    }
    ui->treeActive->scrollToBottom();
    setHint(tr("Added %1.").arg(cm.name));
}

void TxGeneratorWindow::on_lineManualData_textChanged(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        return;
    }
    const auto parsed = HexData::parseByteString(text);
    if (!parsed || parsed->size() > BusMessage::k_maxDataBytes
        || !isCanFdPayloadLength(parsed->size())) {
        return;
    }
    const QSignalBlocker blockDlc(ui->spinManualDlc);
    ui->spinManualDlc->setValue(parsed->size());
}

QList<int> TxGeneratorWindow::selectedRows() const
{
    QList<int> rows;
    for (QTreeWidgetItem *item : ui->treeActive->selectedItems()) {
        const int row = ui->treeActive->indexOfTopLevelItem(item);
        if (row >= 0 && !rows.contains(row)) {
            rows.append(row);
        }
    }
    std::sort(rows.begin(), rows.end());
    return rows;
}

void TxGeneratorWindow::on_btnRemove_released()
{
    removeRows(selectedRows());
}

void TxGeneratorWindow::removeRows(const QList<int> &rows)
{
    if (rows.isEmpty()) {
        setHint(tr("Select at least one row first."));
        return;
    }

    // Remove from the highest index down so the remaining rows keep their indices.
    QList<int> sorted = rows;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    for (int row : sorted) {
        if (row >= 0 && row < _cyclicMessages.size()) {
            _cyclicMessages.removeAt(row);
        }
    }
    updateActiveList();
    setHint(tr("Removed %1 row(s).").arg(sorted.size()));
}

void TxGeneratorWindow::on_btnSendOnce_released()
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty()) {
        const int current = ui->treeActive->currentIndex().row();
        if (current >= 0 && current < _cyclicMessages.size()) {
            rows.append(current);
        }
    }
    sendOnceRows(rows);
}

void TxGeneratorWindow::sendOnceRows(const QList<int> &rows)
{
    if (rows.isEmpty()) {
        setHint(tr("Select at least one row first."));
        return;
    }

    int sent = 0;
    for (int row : rows) {
        if (row < 0 || row >= _cyclicMessages.size()) {
            continue;
        }
        CyclicMessage &cm = _cyclicMessages[row];
        BusInterface *intf = _backend.getInterfaceById(cm.interfaceId);
        if (intf && intf->isOpen()) {
            cm.msg.setInterfaceId(cm.interfaceId);
            intf->sendMessage(cm.msg);
            ++sent;
        } else {
            log_error(tr("TxGeneratorWindow: Interface %1 is not open.")
                          .arg(intf ? intf->getName() : QString::number(cm.interfaceId)));
        }
    }

    _sentFrames += static_cast<quint64>(sent);
    updateRunStatus();
    if (sent == 0) {
        setHint(tr("Nothing was sent: the selected interface is not open."));
    } else {
        setHint(tr("Sent %1 frame(s).").arg(sent));
    }
}

void TxGeneratorWindow::on_btnBulkRun_clicked()
{
    if (!_backend.isMeasurementRunning()) {
        ui->btnBulkRun->setChecked(false);
        setHint(tr("Start the measurement first."));
        return;
    }
    setRowsEnabled(selectedRows(), true);
}

void TxGeneratorWindow::on_btnBulkStop_clicked()
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty()) {
        const int current = ui->treeActive->currentIndex().row();
        if (current >= 0 && current < _cyclicMessages.size()) {
            rows.append(current);
        }
    }
    setRowsEnabled(rows, false);
}

void TxGeneratorWindow::setRowsEnabled(const QList<int> &rows, bool enabled)
{
    if (rows.isEmpty()) {
        setHint(tr("Select at least one row first."));
        return;
    }
    if (enabled && !_backend.isMeasurementRunning()) {
        setHint(tr("Start the measurement first."));
        return;
    }

    for (int row : rows) {
        setEnabled(row, enabled);
    }
    ui->btnBulkRun->setChecked(enabled);
    ui->btnBulkStop->setChecked(!enabled);
    updateSendTimer();
    updateRunStatus();
    setHint(enabled ? tr("Started %1 row(s).").arg(rows.size())
                    : tr("Stopped %1 row(s).").arg(rows.size()));
}

void TxGeneratorWindow::on_spinInterval_valueChanged(int i)
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty()) {
        const int current = ui->treeActive->currentIndex().row();
        if (current >= 0 && current < _cyclicMessages.size()) {
            rows.append(current);
        }
    }
    if (rows.isEmpty()) {
        setHint(tr("Select at least one row first."));
        return;
    }
    for (int row : rows) {
        setInterval(row, i);
    }
    updateSendTimer();
}

void TxGeneratorWindow::on_comboBoxInterface_currentIndexChanged(int index)
{
    (void)index;
    populateDbcMessages();
    emit interfaceChanged(static_cast<BusInterfaceId>(ui->comboBoxInterface->currentData().toUInt()));
}

void TxGeneratorWindow::on_treeAvailable_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    addAvailableMessages({item});
}

void TxGeneratorWindow::on_treeActive_itemSelectionChanged()
{
    const auto selected = ui->treeActive->selectedItems();
    auto *item = selected.value(0, ui->treeActive->currentItem());
    const int row = ui->treeActive->indexOfTopLevelItem(item);
    if (row < 0 || row >= _cyclicMessages.size()) { return; }

    const CyclicMessage &cm = _cyclicMessages.at(row);
    isLoading = true;
    const QSignalBlocker blockRun(ui->btnBulkRun);
    const QSignalBlocker blockStop(ui->btnBulkStop);
    const QSignalBlocker blockInterval(ui->spinInterval);
    ui->btnBulkRun->setChecked(cm.enabled);
    ui->btnBulkStop->setChecked(!cm.enabled);
    ui->spinInterval->setValue(cm.interval);
    isLoading = false;
    if (!selected.isEmpty()) {
        emit messageSelected(cm.msg, cm.name, cm.interfaceId, cm.dbMsg);
    }
}

bool TxGeneratorWindow::currentMessage(BusMessage *msg, QString *name,
                                       BusInterfaceId *interfaceId, CanDbMessage **dbMsg) const
{
    const auto selected = ui->treeActive->selectedItems();
    if (selected.isEmpty()) { return false; }
    const int row = ui->treeActive->indexOfTopLevelItem(selected.first());
    if (row < 0 || row >= _cyclicMessages.size()) { return false; }

    const CyclicMessage &cm = _cyclicMessages.at(row);
    if (msg) { *msg = cm.msg; }
    if (name) { *name = cm.name; }
    if (interfaceId) { *interfaceId = cm.interfaceId; }
    if (dbMsg) { *dbMsg = cm.dbMsg; }
    return true;
}

void TxGeneratorWindow::treeActiveItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    const int row = ui->treeActive->indexOfTopLevelItem(item);
    if (row >= 0) {
        openMessageEditor(row);
    }
}

void TxGeneratorWindow::openMessageEditor(int row)
{
    if (row < 0 || row >= _cyclicMessages.size()) { return; }

    CyclicMessage &cm = _cyclicMessages[row];
    QTreeWidgetItem *item = ui->treeActive->topLevelItem(row);

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Edit Message: %1 - %2").arg(item ? item->text(colId) : QString(), cm.name));
    dlg.resize(750, 480);

    auto *layout = new QVBoxLayout(&dlg);
    auto *rawTx = new RawTxWindow(&dlg, _backend, false);
    rawTx->setMessage(cm.msg, cm.name, cm.interfaceId, cm.dbMsg);
    layout->addWidget(rawTx);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    BusMessage editedMsg = cm.msg;
    BusInterfaceId editedInterfaceId = cm.interfaceId;
    QString editedInterfaceName = cm.interfaceName;

    connect(rawTx, &RawTxWindow::messageUpdated, this, [&editedMsg](const BusMessage &msg) {
        editedMsg = msg;
    });
    connect(rawTx, &RawTxWindow::interfaceSelected, this, [&](BusInterfaceId id) {
        editedInterfaceId = id;
        for (int i = 0; i < ui->comboBoxInterface->count(); ++i) {
            if (static_cast<BusInterfaceId>(ui->comboBoxInterface->itemData(i).toUInt()) == id) {
                editedInterfaceName = ui->comboBoxInterface->itemText(i);
                break;
            }
        }
    });

    if (dlg.exec() == QDialog::Accepted)
    {
        cm.msg           = editedMsg;
        cm.interfaceId   = editedInterfaceId;
        cm.interfaceName = editedInterfaceName;
        updateRowUI(row);
        setHint(tr("Updated %1.").arg(cm.name));
    }
}

void TxGeneratorWindow::showActiveContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeActive->itemAt(pos);
    if (!item) {
        return;
    }
    if (!item->isSelected()) {
        ui->treeActive->clearSelection();
        item->setSelected(true);
        ui->treeActive->setCurrentItem(item);
    }

    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) {
        return;
    }

    _actionEditMessage->setEnabled(rows.size() == 1);
    _actionRename->setEnabled(rows.size() == 1);
    _actionSendOnce->setEnabled(_backend.isMeasurementRunning());
    _actionRun->setEnabled(_backend.isMeasurementRunning());

    QMenu menu(this);
    menu.addAction(_actionEditMessage);
    menu.addSeparator();
    menu.addAction(_actionSendOnce);
    menu.addAction(_actionRun);
    menu.addAction(_actionStop);
    menu.addSeparator();
    menu.addAction(_actionDuplicate);
    menu.addAction(_actionRename);
    menu.addSeparator();
    menu.addAction(_actionRemove);
    menu.exec(ui->treeActive->viewport()->mapToGlobal(pos));
}

void TxGeneratorWindow::duplicateRows(const QList<int> &rows)
{
    if (rows.isEmpty()) {
        setHint(tr("Select at least one row first."));
        return;
    }

    const int firstNew = _cyclicMessages.size();
    for (int row : rows) {
        if (row < 0 || row >= _cyclicMessages.size()) {
            continue;
        }
        CyclicMessage copy = _cyclicMessages.at(row);
        copy.name = tr("%1 copy").arg(copy.name);
        copy.enabled = false;
        copy.nextDue = {};
        _cyclicMessages.append(copy);
    }

    updateActiveList();
    ui->treeActive->clearSelection();
    for (int row = firstNew; row < _cyclicMessages.size(); ++row) {
        if (QTreeWidgetItem *newItem = ui->treeActive->topLevelItem(row)) {
            newItem->setSelected(true);
        }
    }
    ui->treeActive->scrollToBottom();
    setHint(tr("Duplicated %1 row(s).").arg(_cyclicMessages.size() - firstNew));
}

void TxGeneratorWindow::renameRow(int row)
{
    if (row < 0 || row >= _cyclicMessages.size()) {
        return;
    }

    bool ok = false;
    const QString currentName = _cyclicMessages.at(row).name;
    const QString name = QInputDialog::getText(this, tr("Rename Message"), tr("Name:"),
                                               QLineEdit::Normal, currentName, &ok).trimmed();
    if (!ok || name.isEmpty()) {
        return;
    }

    _cyclicMessages[row].name = name;
    updateRowUI(row);
    setHint(tr("Renamed to %1.").arg(name));
}

QString TxGeneratorWindow::dataPreview(const BusMessage &msg)
{
    const int length = msg.getLength();
    if (length <= 0) {
        return QString();
    }
    QStringList bytes;
    bytes.reserve(length);
    for (int i = 0; i < length; ++i) {
        bytes.append(QStringLiteral("%1").arg(msg.getByte(static_cast<uint8_t>(i)), 2, 16,
                                             QLatin1Char('0')).toUpper());
    }
    return bytes.join(QLatin1Char(' '));
}

void TxGeneratorWindow::updateRunStatus()
{
    if (!ui->labelRunStatus) {
        return;
    }
    int running = 0;
    for (const CyclicMessage &cm : std::as_const(_cyclicMessages)) {
        if (cm.enabled) {
            ++running;
        }
    }
    ui->labelRunStatus->setText(tr("Running %1 · sent %2").arg(running).arg(_sentFrames));
}

void TxGeneratorWindow::setHint(const QString &text)
{
    if (!ui->labelManualHint) {
        return;
    }
    ui->labelManualHint->setText(text);
    if (_hintTimer) {
        _hintTimer->start();
    }
}

void TxGeneratorWindow::clearHint()
{
    if (ui->labelManualHint) {
        ui->labelManualHint->clear();
    }
}

void TxGeneratorWindow::on_treeActive_itemChanged(QTreeWidgetItem *item, int column)
{
    int row = ui->treeActive->indexOfTopLevelItem(item);
    if (row >= 0 && row < _cyclicMessages.size()) {
        if (column == colInterval) {
            bool ok;
            int interval = item->text(colInterval).toInt(&ok);
            if (ok && interval > 0 && _cyclicMessages[row].interval != interval) {
                setInterval(row, interval);

                // If this item is part of a selection, apply to all selected items
                if (item->isSelected()) {
                    for (auto *selItem : ui->treeActive->selectedItems()) {
                        if (selItem == item) continue;
                        int selRow = ui->treeActive->indexOfTopLevelItem(selItem);
                        if (selRow >= 0 && selRow < _cyclicMessages.size()) {
                            setInterval(selRow, interval);
                        }
                    }
                }
                updateSendTimer();
            }
        }
    }
}

void TxGeneratorWindow::on_btnSelectAll_released()
{
    ui->treeActive->selectAll();
}

void TxGeneratorWindow::on_btnClearAll_released()
{
    ui->treeActive->clearSelection();
}

void TxGeneratorWindow::onStatusButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // Determine the row of the button
    QPoint pos = btn->parentWidget()->mapTo(ui->treeActive->viewport(), btn->pos());
    QTreeWidgetItem *item = ui->treeActive->itemAt(pos);
    if (!item) return;

    int row = ui->treeActive->indexOfTopLevelItem(item);
    if (row >= 0 && row < _cyclicMessages.size()) {
        bool targetState = !_cyclicMessages[row].enabled;

        // If this item is part of a selection, apply to all selected items
        if (item->isSelected()) {
            for (auto *selItem : ui->treeActive->selectedItems()) {
                int selRow = ui->treeActive->indexOfTopLevelItem(selItem);
                if (selRow >= 0 && selRow < _cyclicMessages.size()) {
                    setEnabled(selRow, targetState);
                }
            }
        } else {
            setEnabled(row, targetState);
        }
        updateSendTimer();
    }
}

void TxGeneratorWindow::onSendTimerTimeout()
{
    if (!_backend.isMeasurementRunning()) {
        _sendTimer->stop();
        return;
    }
    const auto now = TxClock::now();

    for (int i = 0; i < _cyclicMessages.size(); ++i) {
        CyclicMessage &cm = _cyclicMessages[i];
        if (!cm.enabled) { continue; }

        const auto period = std::chrono::milliseconds(std::max(1, cm.interval));
        if (cm.nextDue == TxClock::time_point{}) { cm.nextDue = now; } // first send after enable
        if (cm.nextDue > now) { continue; }

        BusInterface *intf = _backend.getInterfaceById(cm.interfaceId);
        if (intf && intf->isOpen()) {
            cm.msg.setInterfaceId(cm.interfaceId);
            intf->sendMessage(cm.msg);
            ++_sentFrames;

            // Advance from the scheduled deadline, never from the actual send
            // time, so event-loop latency cannot accumulate into cycle drift.
            cm.nextDue += period;
            if (cm.nextDue <= now) {
                // More than one period behind (event loop stalled, or the cycle
                // time was shortened): skip the missed slots instead of bursting.
                const auto missed = (now - cm.nextDue) / period + 1;
                cm.nextDue += period * missed;
            }
        } else {
            // Disable to avoid error spam; user must re-enable manually
            cm.enabled = false;
            updateRowUI(i);
            log_error(QString("TxGeneratorWindow: Interface %1 is not open.")
                .arg(intf ? intf->getName() : QString::number(cm.interfaceId)));
        }
    }
    updateRunStatus();
    updateSendTimer();
}

void TxGeneratorWindow::onSetupChanged()
{
    for (CyclicMessage &cm : _cyclicMessages) {
        cm.dbMsg = nullptr;
    }
    refreshInterfaces();
}

void TxGeneratorWindow::updateMeasurementState()
{
    // The active list stays editable without a measurement (add, remove, edit,
    // change interval); only actions that put frames on the bus need one.
    bool running = _backend.isMeasurementRunning();
    ui->btnSendOnce->setEnabled(running);
    ui->btnBulkRun->setEnabled(running);
    if (!running) {
        stopAll();
    }
    for (int i = 0; i < _cyclicMessages.size(); ++i) {
        updateRowUI(i);
    }
    updateSendTimer();
}

void TxGeneratorWindow::updateActiveList()
{
    // Save selection
    QList<int> selected = selectedRows();
    int currentRow = ui->treeActive->currentIndex().row();

    ui->treeActive->blockSignals(true);
    ui->treeActive->clear();
    for (int i = 0; i < _cyclicMessages.size(); ++i) {
        const CyclicMessage &cm = _cyclicMessages[i];
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeActive);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable); // Explicitly remove checkbox

        // Create Status Button
        const ThemeColors &colors = ThemeManager::instance().colors();
        QPushButton *btnStatus = new QPushButton(cm.enabled ? QStringLiteral("■") : QStringLiteral("▶"));
        btnStatus->setToolTip(cm.enabled ? tr("Stop") : tr("Start"));
        btnStatus->setFixedWidth(40);
        const QString accent = cm.enabled ? colors.statusError.name() : colors.statusSuccess.name();
        btnStatus->setStyleSheet(QStringLiteral(
            "QPushButton { color: %1; font-weight: bold; background: transparent; border: 1px solid %1; border-radius: 3px; }"
            "QPushButton:hover { background: %1; color: white; }").arg(accent));

        // Starting needs a running measurement; stopping is always allowed.
        btnStatus->setEnabled(cm.enabled || _backend.isMeasurementRunning());
        connect(btnStatus, &QPushButton::clicked, this, &TxGeneratorWindow::onStatusButtonClicked);
        ui->treeActive->setItemWidget(item, colStatus, btnStatus);

        item->setText(colId, "0x" + QString("%1").arg(cm.msg.getId(), 3, 16, QChar('0')).toUpper());
        item->setText(colName, cm.name);
        item->setText(colData, dataPreview(cm.msg));
        BusInterface *intf = _backend.getInterfaceById(cm.interfaceId);
        item->setText(colInterface, intf ? intf->getName() : tr("Unknown"));
        item->setText(colDlc, QString::number(cm.msg.getLength()));
        item->setText(colInterval, QString::number(cm.interval));
        item->setToolTip(colData, dataPreview(cm.msg));

        if (selected.contains(i)) {
            item->setSelected(true);
        }
        if (i == currentRow) {
            ui->treeActive->setCurrentItem(item);
        }
    }
    ui->treeActive->blockSignals(false);
    updateRunStatus();
}

void TxGeneratorWindow::updateRowUI(int row)
{
    if (row < 0 || row >= _cyclicMessages.size()) return;
    QTreeWidgetItem *item = ui->treeActive->topLevelItem(row);
    if (!item) return;

    const CyclicMessage &cm = _cyclicMessages[row];

    ui->treeActive->blockSignals(true);

    // Update the status button in the Status column
    QPushButton *btnStatus = qobject_cast<QPushButton*>(ui->treeActive->itemWidget(item, colStatus));
    if (btnStatus) {
        const ThemeColors &colors = ThemeManager::instance().colors();
        btnStatus->setText(cm.enabled ? QStringLiteral("■") : QStringLiteral("▶"));
        btnStatus->setToolTip(cm.enabled ? tr("Stop") : tr("Start"));
        const QString accent = cm.enabled ? colors.statusError.name() : colors.statusSuccess.name();
        btnStatus->setStyleSheet(QStringLiteral(
            "QPushButton { color: %1; font-weight: bold; background: transparent; border: 1px solid %1; border-radius: 3px; }"
            "QPushButton:hover { background: %1; color: white; }").arg(accent));
        btnStatus->setEnabled(cm.enabled || _backend.isMeasurementRunning());
    }

    item->setText(colId, "0x" + QString("%1").arg(cm.msg.getId(), 3, 16, QChar('0')).toUpper());
    item->setText(colName, cm.name);
    item->setText(colData, dataPreview(cm.msg));
    BusInterface *intf = _backend.getInterfaceById(cm.interfaceId);
    item->setText(colInterface, intf ? intf->getName() : tr("Unknown"));
    item->setText(colDlc, QString::number(cm.msg.getLength()));
    item->setText(colInterval, QString::number(cm.interval));
    item->setToolTip(colData, dataPreview(cm.msg));

    ui->treeActive->blockSignals(false);
    updateRunStatus();
}


void TxGeneratorWindow::stopAll()
{
    for (int i = 0; i < _cyclicMessages.size(); ++i) {
        _cyclicMessages[i].enabled = false;
        updateRowUI(i);
    }
    ui->btnBulkRun->setChecked(false);
    ui->btnBulkStop->setChecked(false);
    updateRunStatus();
}

QSize TxGeneratorWindow::sizeHint() const
{
    return QSize(1200, 400);
}

void TxGeneratorWindow::setInterval(int row, int interval_ms)
{
    CyclicMessage &cm = _cyclicMessages[row];
    cm.interval = interval_ms;

    // A shorter cycle time must take effect now, not after the deadline that was
    // armed for the old (longer) one.
    const auto latest = TxClock::now() + std::chrono::milliseconds(std::max(1, interval_ms));
    if (cm.nextDue > latest) { cm.nextDue = latest; }

    updateRowUI(row);
}

void TxGeneratorWindow::setEnabled(int row, bool enabled)
{
    CyclicMessage &cm = _cyclicMessages[row];
    if (enabled && !_backend.isMeasurementRunning()) { return; } // nothing can be sent
    if (enabled && !cm.enabled) { cm.nextDue = {}; } // fresh schedule, send immediately
    cm.enabled = enabled;
    updateRowUI(row);
}

void TxGeneratorWindow::updateSendTimer()
{
    if (!_backend.isMeasurementRunning()) {
        _sendTimer->stop();
        return;
    }

    const auto now = TxClock::now();
    std::optional<TxClock::time_point> earliest;
    for (const CyclicMessage &cm : std::as_const(_cyclicMessages)) {
        if (!cm.enabled) { continue; }
        // An unscheduled message is due right away.
        const auto due = (cm.nextDue == TxClock::time_point{}) ? now : cm.nextDue;
        if (!earliest || due < *earliest) { earliest = due; }
    }

    if (!earliest) {
        _sendTimer->stop();
        return;
    }

    // Round up so the timer never fires before the deadline, which would only
    // cost an extra wakeup and re-arm.
    const auto delay = std::chrono::ceil<std::chrono::milliseconds>(*earliest - now);
    _sendTimer->start(std::max<qint64>(0, delay.count()));
}
