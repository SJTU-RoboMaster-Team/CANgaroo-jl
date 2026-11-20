/*

  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
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

#include "TraceWindow.h"
#include "ui_TraceWindow.h"

#include <QCheckBox>
#include <QDomDocument>
#include <QHeaderView>
#include <QMenu>
#include <QScrollBar>
#include <QSettings>
#include <QSortFilterProxyModel>

#include "core/Backend.h"
#include "core/BusTrace.h"
#include "core/ThemeManager.h"
#include "core/TriggerCaptureManager.h"
#include "helpers/apphelpers.h"
#include "window/TriggerCaptureDialog/TriggerCaptureDialog.h"

#include "AggregatedTraceViewModel.h"
#include "DataColumnDelegate.h"
#include "TraceFilterDialog.h"
#include "TraceFilterModel.h"
#include "UnifiedTraceViewModel.h"


TraceWindow::TraceWindow(QWidget *parent, Backend &backend) :
    ConfigurableWidget(parent),
    ui(new Ui::TraceWindow),
    _backend(&backend),
    _timestampMode(timestamp_mode_absolute)
{
    ui->setupUi(this);

    applyIcons();
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](ThemeManager::Theme) { applyIcons(); });

    _aggregatedTraceViewModel = new AggregatedTraceViewModel(backend);
    _aggregatedProxyModel = new QSortFilterProxyModel(this);
    _aggregatedProxyModel->setSourceModel(_aggregatedTraceViewModel);
    _aggregatedProxyModel->setDynamicSortFilter(false);

    _aggMonitorFilterModel = new TraceFilterModel(this);
    _aggMonitorFilterModel->setSourceModel(_aggregatedProxyModel);

    _rollingModel = new UnifiedTraceViewModel(backend);
    _rollingFilterModel = new TraceFilterModel(this);
    _rollingFilterModel->setSourceModel(_rollingModel);

    QTreeView *tree = ui->treeAgg;
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    tree->setFont(font);
    tree->setAlternatingRowColors(true);
    tree->setUniformRowHeights(true);
    tree->setRootIsDecorated(true);
    tree->header()->setDefaultAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    tree->setColumnWidth(BaseTraceViewModel::column_index, 70);
    tree->setColumnWidth(BaseTraceViewModel::column_timestamp, 100);
    tree->setColumnWidth(BaseTraceViewModel::column_channel, 120);
    tree->setColumnWidth(BaseTraceViewModel::column_direction, 56);
    tree->setColumnWidth(BaseTraceViewModel::column_type, 80);
    tree->setColumnWidth(BaseTraceViewModel::column_canid, 110);
    tree->setColumnWidth(BaseTraceViewModel::column_sender, 150);
    tree->setColumnWidth(BaseTraceViewModel::column_name, 150);
    tree->setColumnWidth(BaseTraceViewModel::column_dlc, 50);
    tree->setColumnWidth(BaseTraceViewModel::column_data, 260);
    tree->setColumnWidth(BaseTraceViewModel::column_comment, 120);
    tree->setItemDelegateForColumn(BaseTraceViewModel::column_data, new DataColumnDelegate(tree));
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, &QTreeView::customContextMenuRequested, this, &TraceWindow::onTreeContextMenu);

    connect(_rollingFilterModel, &QAbstractItemModel::rowsInserted, this, &TraceWindow::onRowsInserted);
    connect(_aggMonitorFilterModel, &QAbstractItemModel::rowsInserted, this, &TraceWindow::onRowsInserted);

    ui->cbViewMode->addItem(tr("Aggregated"), mode_aggregated);
    ui->cbViewMode->addItem(tr("Rolling Log"), mode_unified);

    ui->cbTimestampMode->addItem(tr("Absolute"), timestamp_mode_absolute);
    ui->cbTimestampMode->addItem(tr("Absolute (UTC)"), timestamp_mode_absolute_utc);
    ui->cbTimestampMode->addItem(tr("Relative"), timestamp_mode_relative);
    ui->cbTimestampMode->addItem(tr("Delta"), timestamp_mode_delta);

    QSettings appSettings;
    const auto defaultTsMode = static_cast<timestamp_mode_t>(
        appSettings.value("tracewindow/defaultTimestampMode", timestamp_mode_delta).toInt());
    setTimestampMode(defaultTsMode);

    const auto defaultViewMode = static_cast<mode_t>(
        appSettings.value("tracewindow/defaultViewMode", mode_aggregated).toInt());
    _mode = defaultViewMode;

    // Debounce typing so the filter models are not re-evaluated on every keystroke
    _filterDebounceTimer.setInterval(250);
    _filterDebounceTimer.setSingleShot(true);
    connect(&_filterDebounceTimer, &QTimer::timeout, this, &TraceWindow::on_cbFilterChanged);
    connect(ui->filterLineEdit, &QLineEdit::textChanged, &_filterDebounceTimer, qOverload<>(&QTimer::start));
    connect(ui->TraceClearpushButton, &QPushButton::released, this, &TraceWindow::on_cbTraceClearpushButton);
    connect(ui->cbViewMode, &QComboBox::currentIndexChanged, this, &TraceWindow::on_cbViewMode_currentIndexChanged);
    connect(ui->filterButton, &QPushButton::clicked, this, &TraceWindow::openFilterDialog);

    _scrollTimer.setInterval(100);
    _scrollTimer.setSingleShot(true);
    connect(&_scrollTimer, &QTimer::timeout, this, &TraceWindow::doScrollToBottom);

    setMode(_mode);
}

TraceWindow::~TraceWindow()
{
    delete ui;
    delete _aggregatedTraceViewModel;
    delete _rollingModel;
}

void TraceWindow::retranslateUi()
{
    ui->retranslateUi(this);
}

void TraceWindow::onTreeContextMenu(const QPoint &pos)
{
    auto *tree = qobject_cast<QTreeView *>(sender());
    if (!tree || !_backend) return;

    const QModelIndex idx = tree->indexAt(pos);
    if (!idx.isValid()) return;

    const QModelIndex idIdx = idx.sibling(idx.row(), BaseTraceViewModel::column_canid);
    QString idText = idIdx.data(Qt::DisplayRole).toString().trimmed();
    if (idText.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive)) {
        idText = idText.mid(2);
    }
    bool ok = false;
    const uint32_t canId = idText.toUInt(&ok, 16);
    if (!ok) return;

    QMenu menu(this);
    QAction *actFrame = menu.addAction(tr("Create Trigger Rule for ID 0x%1...").arg(canId, 0, 16).toUpper());
    QAction *actWatchdog = menu.addAction(tr("Create Watchdog Trigger (200 ms) for ID 0x%1...").arg(canId, 0, 16).toUpper());
    QAction *chosen = menu.exec(tree->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    auto *mgr = _backend->getTriggerCaptureManager();
    if (!mgr) return;

    TriggerCaptureConfig cfg = mgr->config();
    if (chosen == actFrame) {
        cfg.rules.append(TriggerCaptureDialog::makeQuickFrameRule(canId));
    } else if (chosen == actWatchdog) {
        cfg.rules.append(TriggerCaptureDialog::makeQuickWatchdogRule(canId, 200));
    }

    TriggerCaptureDialog dlg(cfg, mgr->isArmed(), this);
    if (dlg.exec() == QDialog::Accepted) {
        mgr->setConfig(dlg.config());
        mgr->setArmed(dlg.shouldArm());
    }
}

void TraceWindow::setMode(TraceWindow::mode_t mode)
{
    _mode = mode;

    QTreeView *tree = ui->treeAgg;
    if (mode == mode_aggregated) {
        tree->setModel(_aggMonitorFilterModel);
        tree->setSortingEnabled(true);
        tree->sortByColumn(BaseTraceViewModel::column_canid, Qt::AscendingOrder);
        tree->setRootIsDecorated(true);
    } else {
        tree->setSortingEnabled(false);
        tree->header()->setSortIndicator(-1, Qt::AscendingOrder);
        _rollingFilterModel->sort(-1);
        tree->setModel(_rollingFilterModel);
        tree->setRootIsDecorated(true);
    }

    for (int i = 0; i < ui->cbViewMode->count(); i++) {
        if (ui->cbViewMode->itemData(i).toInt() == mode) {
            ui->cbViewMode->setCurrentIndex(i);
            break;
        }
    }
    tree->scrollToBottom();
}

void TraceWindow::setTimestampMode(int mode)
{
    timestamp_mode_t new_mode;
    if ( (mode>=0) && (mode<timestamp_modes_count) )
    {
        new_mode = (timestamp_mode_t) mode;
    }
    else
    {
        new_mode = timestamp_mode_absolute;
    }

    _aggregatedTraceViewModel->setTimestampMode(new_mode);
    _rollingModel->setTimestampMode(new_mode);

    if (new_mode != _timestampMode)
    {
        _timestampMode = new_mode;
        for (int i=0; i<ui->cbTimestampMode->count(); i++)
        {
            if (ui->cbTimestampMode->itemData(i).toInt() == new_mode)
            {
                ui->cbTimestampMode->setCurrentIndex(i);
            }
        }
        emit(settingsChanged(this));
    }
}

bool TraceWindow::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    if (!ConfigurableWidget::saveXML(backend, xml, root))
    {
        return false;
    }

    root.setAttribute("type", "TraceWindow");
    root.setAttribute("mode", _mode == mode_unified ? "unified" : "aggregated");
    root.setAttribute("TimestampMode", _timestampMode);

    QDomElement elAggregated = xml.createElement("AggregatedTraceView");
    elAggregated.setAttribute("SortColumn", _aggregatedProxyModel->sortColumn());
    root.appendChild(elAggregated);

    QDomElement filterEl = xml.createElement("TraceFilter");
    filterEl.setAttribute("showTx", _filterShowTx ? "1" : "0");
    filterEl.setAttribute("showRx", _filterShowRx ? "1" : "0");

    QStringList hiddenMsgs;
    for (uint32_t id : _filterHiddenMessageIds)
    {
        hiddenMsgs.append(QString::number(id));
    }
    if (!hiddenMsgs.isEmpty())
    {
        filterEl.setAttribute("hiddenMessages", hiddenMsgs.join(","));
    }

    QStringList hiddenIfs;
    for (BusInterfaceId id : _filterHiddenInterfaces)
    {
        hiddenIfs.append(QString::number(id));
    }
    if (!hiddenIfs.isEmpty())
    {
        filterEl.setAttribute("hiddenInterfaces", hiddenIfs.join(","));
    }

    root.appendChild(filterEl);

    return true;
}

bool TraceWindow::loadXML(Backend &backend, QDomElement &el)
{
    if (!ConfigurableWidget::loadXML(backend, el))
    {
        return false;
    }

    QString modeStr = el.attribute("mode", "aggregated");
    _mode = (modeStr == "unified") ? mode_unified : mode_aggregated;
    setMode(_mode);
    setTimestampMode(el.attribute("TimestampMode", "0").toInt());

    QDomElement elAggregated = el.firstChildElement("AggregatedTraceView");
    int sortColumn = elAggregated.attribute("SortColumn", "-1").toInt();
    ui->treeAgg->sortByColumn(sortColumn, Qt::AscendingOrder);

    QDomElement filterEl = el.firstChildElement("TraceFilter");
    if (!filterEl.isNull())
    {
        _filterShowTx = filterEl.attribute("showTx", "1") == "1";
        _filterShowRx = filterEl.attribute("showRx", "1") == "1";

        QString hiddenMsgsStr = filterEl.attribute("hiddenMessages");
        if (!hiddenMsgsStr.isEmpty())
        {
            for (const QString &s : hiddenMsgsStr.split(","))
            {
                _filterHiddenMessageIds.insert(s.toUInt());
            }
        }

        QString hiddenIfsStr = filterEl.attribute("hiddenInterfaces");
        if (!hiddenIfsStr.isEmpty())
        {
            for (const QString &s : hiddenIfsStr.split(","))
            {
                _filterHiddenInterfaces.insert(static_cast<BusInterfaceId>(s.toUInt()));
            }
        }

        applyDialogFilters();
    }

    return true;
}

void TraceWindow::addMessage(const BusMessage &msg)
{
    _backend->getTrace()->enqueueMessage(msg);
}

void TraceWindow::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    (void) parent;
    (void) first;
    (void) last;

    if (!ui->cbAutoscroll->isChecked()) { return; }

    _scrollPending = true;

    if (!_scrollTimer.isActive()) {
        _scrollTimer.start();
    }

    if(_backend->getTrace()->size() > 1000000)
    {
        _backend->clearTrace();
    }
}

void TraceWindow::doScrollToBottom()
{
    if (_scrollPending) {
        ui->treeAgg->scrollToBottom();
        _scrollPending = false;
    }
}

void TraceWindow::on_cbTimestampMode_currentIndexChanged(int index)
{
    setTimestampMode((timestamp_mode_t)ui->cbTimestampMode->itemData(index).toInt());
}

void TraceWindow::on_cbFilterChanged()
{
    QString filterText = ui->filterLineEdit->text();
    _aggMonitorFilterModel->setFilterText(filterText);
    _aggMonitorFilterModel->invalidate();

    _rollingFilterModel->setFilterText(filterText);
    _rollingFilterModel->invalidate();
}

void TraceWindow::on_cbTraceClearpushButton()
{
    _backend->clearTrace();
}

void TraceWindow::on_cbViewMode_currentIndexChanged(int index)
{
    setMode((mode_t)ui->cbViewMode->itemData(index).toInt());
}

void TraceWindow::applyIcons()
{
    // The bundled SVGs draw with stroke="currentColor", which QSvgRenderer does
    // not resolve — it paints them black, so they vanish against a dark theme.
    // Recolor them to the palette's text color instead of using the SVG as-is.
    ui->filterButton->setIcon(AppHelpers::themedIcon("view-filter", ":/assets/filter-symbolic.svg"));
    ui->TraceClearpushButton->setIcon(AppHelpers::themedIcon("user-trash-full",
                                                            ":/assets/icons/edit-clear-symbolic.svg"));
}

void TraceWindow::openFilterDialog()
{
    TraceFilterDialog dlg(*_backend, this);
    dlg.setShowTx(_filterShowTx);
    dlg.setShowRx(_filterShowRx);
    dlg.setHiddenMessageIds(_filterHiddenMessageIds);
    dlg.setHiddenInterfaces(_filterHiddenInterfaces);

    if (dlg.exec() == QDialog::Accepted)
    {
        _filterShowTx = dlg.showTx();
        _filterShowRx = dlg.showRx();
        _filterHiddenMessageIds = dlg.hiddenMessageIds();
            _filterHiddenInterfaces = dlg.hiddenInterfaces();

        applyDialogFilters();
        emit settingsChanged(this);
    }
}

void TraceWindow::applyDialogFilters()
{
    auto applyToModel = [&](TraceFilterModel *model)
    {
        model->setShowTx(_filterShowTx);
        model->setShowRx(_filterShowRx);
        model->setHiddenMessageIds(_filterHiddenMessageIds);
        model->setHiddenInterfaces(_filterHiddenInterfaces);
        model->invalidate();
    };

    applyToModel(_rollingFilterModel);
    applyToModel(_aggMonitorFilterModel);
}

