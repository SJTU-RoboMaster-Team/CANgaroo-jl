/*

  Copyright (c) 2026 Jayachandran Dharuman
  Copyright (c) 2026 Schildkroet

  This file is part of CANgaroo.

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

#include "core/Backend.h"
#include "core/ConfigurableWidget.h"
#include "core/MeasurementSetup.h"
#include <chrono>

#include <QList>
#include <QTreeWidgetItem>


namespace Ui {
class TxGeneratorWindow;
}

#include "BitMatrixWidget.h"
#include "db/model/CanDbMessage.h"
#include "db/model/CanDbSignal.h"

class QAction;
class QDomDocument;
class QDomElement;

class TxGeneratorWindow : public ConfigurableWidget
{
    Q_OBJECT
public:
    explicit TxGeneratorWindow(QWidget *parent, Backend &backend);
    ~TxGeneratorWindow();

    virtual bool saveXML(Backend &backend, QDomDocument &xml, QDomElement &root);
    virtual bool loadXML(Backend &backend, QDomElement &el);
    virtual QSize sizeHint() const override;
    [[nodiscard]] bool currentMessage(BusMessage *msg, QString *name,
                                      BusInterfaceId *interfaceId, CanDbMessage **dbMsg) const;

protected:
    void retranslateUi() override;

signals:
    void loopbackFrame(const BusMessage &msg);
    void interfaceChanged(BusInterfaceId interfaceId);
    void messageSelected(const BusMessage &msg, const QString &name, BusInterfaceId interfaceId, CanDbMessage *dbMsg);

public slots:
    void stopAll();

private slots:
    void on_lineEditSearchAvailable_textChanged(const QString &text);
    void on_sliderLayoutZoom_valueChanged(int value);
    void on_cbLayoutCompact_toggled(bool checked);
    void on_btnAddToList_released();
    void on_btnAddManual_released();
    void on_btnRemove_released();
    void on_btnSendOnce_released();
    void on_btnBulkRun_clicked();
    void on_btnBulkStop_clicked();
    void on_spinInterval_valueChanged(int i);
    void on_comboBoxInterface_currentIndexChanged(int index);
    void on_treeAvailable_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeAvailable_itemSelectionChanged();
    void on_treeActive_itemSelectionChanged();
    void treeActiveItemDoubleClicked(QTreeWidgetItem *item, int column);
    void showActiveContextMenu(const QPoint &pos);
    /// Keeps the DLC spin box in sync when a valid payload is typed.
    void on_lineManualData_textChanged(const QString &text);
    void onSendTimerTimeout();
    void onSetupChanged();
    void on_btnSelectAll_released();
    void on_btnClearAll_released();
    void on_treeActive_itemChanged(QTreeWidgetItem *item, int column);
    void onStatusButtonClicked();
    void updateMeasurementState();
    void refreshInterfaces();

private:
    enum Column {
        colStatus = 0,
        colId,
        colName,
        colData,
        colInterface,
        colDlc,
        colInterval,
        colCount
    };

    Ui::TxGeneratorWindow *ui;
    Backend &_backend;
    QTimer *_sendTimer;
    BitMatrixWidget *_bitMatrixWidget;
    QTimer *_hintTimer = nullptr;

    QAction *_actionEditMessage = nullptr;
    QAction *_actionSendOnce = nullptr;
    QAction *_actionRun = nullptr;
    QAction *_actionStop = nullptr;
    QAction *_actionDuplicate = nullptr;
    QAction *_actionRename = nullptr;
    QAction *_actionRemove = nullptr;

    /// Cumulative number of frames submitted by this Generator window.
    quint64 _sentFrames = 0;

    /// Monotonic clock for the TX schedule; unaffected by wall-clock (NTP) steps.
    using TxClock = std::chrono::steady_clock;

    struct CyclicMessage {
        BusMessage msg;
        QString name;
        int interval;
        bool enabled;
        /// Absolute deadline of the next send. A default-constructed value means
        /// "not scheduled yet" and makes the message due immediately.
        TxClock::time_point nextDue;
        BusInterfaceId interfaceId;
        CanDbMessage *dbMsg;
        QString interfaceName; ///< Display name saved to XML; resolved to interfaceId by resolveInterfaceNames()
    };

    QList<CyclicMessage> _cyclicMessages;

    bool isLoading;
    void updateAvailableList();
    void updateActiveList();
    void updateRowUI(int row);
    void populateDbcMessages();
    // Matches each CyclicMessage::interfaceName against the combo box and
    // sets interfaceId accordingly. Called after the combo box is repopulated.
    void resolveInterfaceNames();
    // Re-links each CyclicMessage without a dbMsg to its CanDbMessage by raw CAN
    // ID, via the setup's message cache. Needed after loadXML(), since dbMsg is
    // a runtime pointer that isn't (and can't be) persisted to the project file.
    void resolveDbMessages();
    /// Arms the single-shot send timer for the earliest pending deadline, or
    /// stops it when nothing is due.
    void updateSendTimer();
    /// Applies a new cycle time to a row and pulls its pending deadline in if
    /// the old (longer) one is still armed.
    void setInterval(int row, int interval_ms);
    /// Enables/disables a row, restarting its schedule on the off->on edge.
    void setEnabled(int row, bool enabled);

    // ── Generator View usability helpers ────────────────────────────────────
    /// Adds one or more DBC messages to the active list and selects the new rows.
    void addAvailableMessages(const QList<QTreeWidgetItem *> &items);
    /// Validates the manual entry fields, appends the message and reports errors
    /// in the hint label instead of failing silently.
    void addManualMessage();
    /// Opens the embedded Message View editor for one active row.
    void openMessageEditor(int row);
    /// Sends the selected rows once and updates the frame counter.
    void sendOnceRows(const QList<int> &rows);
    void setRowsEnabled(const QList<int> &rows, bool enabled);
    void duplicateRows(const QList<int> &rows);
    void renameRow(int row);
    void removeRows(const QList<int> &rows);
    /// Rows currently selected in the active tree (in list order).
    [[nodiscard]] QList<int> selectedRows() const;
    /// Space-separated upper-case hex preview of the payload.
    [[nodiscard]] static QString dataPreview(const BusMessage &msg);
    void updateRunStatus();
    void setHint(const QString &text);
    void clearHint();
};
