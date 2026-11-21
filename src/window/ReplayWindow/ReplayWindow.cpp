/*
  Copyright (c) 2026 Schildkroet
  Copyright (c) 2026 SJTU RoboMaster Team

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
#include "ReplayWindow.h"

#include <algorithm>
#include <cmath>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDomDocument>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSet>
#include <QSlider>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "helpers/appfonts.h"
#include <QVBoxLayout>

#include "core/Backend.h"
#include "core/BusTrace.h"
#include "core/Log.h"
#include "core/TraceFileFormat.h"
#include "core/TraceFileReader.h"
#include "db/model/CanDbMessage.h"
#include "driver/BusInterface.h"

ReplayWindow::ReplayWindow(QWidget *parent, Backend &backend)
    : ConfigurableWidget(parent)
    , _backend(&backend)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    // Media transport toolbar
    auto *toolbar = new QHBoxLayout();
    toolbar->setSpacing(4);
    _btnLoad = new QPushButton(tr("Load..."), this);
    _btnPlayPause = new QPushButton(tr("Play"), this);
    _btnStop = new QPushButton(tr("Stop"), this);
    _btnStep = new QPushButton(tr("Step +1"), this);
    _btnPlayPause->setEnabled(false);
    _btnStop->setEnabled(false);
    _btnStep->setEnabled(false);

    _speedLabel = new QLabel(tr("Speed:"), this);
    _speedSpin = new QDoubleSpinBox(this);
    _speedSpin->setRange(0.1, 20.0);
    _speedSpin->setValue(1.0);
    _speedSpin->setSingleStep(0.1);
    _speedSpin->setSuffix(QStringLiteral("x"));
    _speedSpin->setToolTip(tr("Playback speed multiplier"));

    _cbAutoplay = new QCheckBox(tr("Autoplay"), this);
    _cbAutoplay->setToolTip(tr("Automatically start/stop replay with measurement"));

    _cbLoop = new QCheckBox(tr("Loop"), this);
    _cbLoop->setToolTip(tr("Restart replay after all messages were sent"));

    toolbar->addWidget(_btnLoad);
    toolbar->addWidget(_btnPlayPause);
    toolbar->addWidget(_btnStop);
    toolbar->addWidget(_btnStep);
    toolbar->addWidget(_cbAutoplay);
    toolbar->addWidget(_cbLoop);
    toolbar->addStretch();
    toolbar->addWidget(_speedLabel);
    toolbar->addWidget(_speedSpin);
    mainLayout->addLayout(toolbar);

    // Offline actions row (Load to Trace / Split Save)
    auto *fileRow = new QHBoxLayout();
    fileRow->setSpacing(4);
    _fileLabel = new QLineEdit(this);
    _fileLabel->setReadOnly(true);
    _fileLabel->setPlaceholderText(tr("No trace file loaded"));
    _btnLoadToTrace = new QPushButton(tr("Load to Trace"), this);
    _btnLoadToTrace->setToolTip(tr("Import enabled messages directly into Trace and Graph"));
    _btnLoadToTrace->setEnabled(false);
    _btnSplitSave = new QPushButton(tr("Split Save..."), this);
    _btnSplitSave->setToolTip(tr("Split large trace into smaller files by frame count"));
    _btnSplitSave->setEnabled(false);
    fileRow->addWidget(_fileLabel, 1);
    fileRow->addWidget(_btnLoadToTrace);
    fileRow->addWidget(_btnSplitSave);
    mainLayout->addLayout(fileRow);

    // Slider + position/time label
    auto *sliderLayout = new QHBoxLayout();
    _slider = new QSlider(Qt::Horizontal, this);
    _slider->setEnabled(false);
    _posLabel = new QLabel(QStringLiteral("00:00.000 / 00:00.000 (0 / 0)"), this);
    _posLabel->setMinimumWidth(190);
    sliderLayout->addWidget(_slider, 1);
    sliderLayout->addWidget(_posLabel);
    mainLayout->addLayout(sliderLayout);

    _progressBar = new QProgressBar(this);
    _progressBar->setRange(0, 100);
    _progressBar->setValue(0);
    _progressBar->setFormat(QStringLiteral("%p%"));
    _progressBar->setTextVisible(true);
    _progressBar->setMaximumHeight(14);
    mainLayout->addWidget(_progressBar);

    QFont mono = AppFonts::monoFont(QApplication::font().pointSizeF());
    mono.setStyleHint(QFont::TypeWriter);

    _infoBox = new QPlainTextEdit(this);
    _infoBox->setFont(mono);
    _infoBox->setReadOnly(true);
    _infoBox->setPlaceholderText(tr("Trace file info..."));
    _infoBox->setMaximumHeight(72);
    mainLayout->addWidget(_infoBox);

    // Filter tree with select/deselect buttons
    auto *filterBtnLayout = new QHBoxLayout();
    _btnSelectAll = new QPushButton(tr("Select All"), this);
    _btnDeselectAll = new QPushButton(tr("Deselect All"), this);
    filterBtnLayout->addWidget(_btnSelectAll);
    filterBtnLayout->addWidget(_btnDeselectAll);
    filterBtnLayout->addStretch();
    mainLayout->addLayout(filterBtnLayout);

    _filterTree = new QTreeWidget(this);
    _filterTree->setHeaderLabels({tr("Interface / CAN ID"), tr("Name"), tr("RX"), tr("TX"), tr("Count"), tr("Output")});
    _filterTree->header()->setStretchLastSection(false);
    _filterTree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _filterTree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    _filterTree->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _filterTree->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    _filterTree->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    _filterTree->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    mainLayout->addWidget(_filterTree, 1);

    _timer = new QTimer(this);
    _timer->setTimerType(Qt::PreciseTimer);

    connect(_btnLoad, &QPushButton::clicked, this, &ReplayWindow::onLoadClicked);
    connect(_btnPlayPause, &QPushButton::clicked, this, &ReplayWindow::onPlayPauseClicked);
    connect(_btnStop, &QPushButton::clicked, this, &ReplayWindow::onStopClicked);
    connect(_btnStep, &QPushButton::clicked, this, &ReplayWindow::onStepClicked);
    connect(_btnLoadToTrace, &QPushButton::clicked, this, &ReplayWindow::onLoadToTraceClicked);
    connect(_btnSplitSave, &QPushButton::clicked, this, &ReplayWindow::onSplitSaveClicked);
    connect(_slider, &QSlider::sliderMoved, this, &ReplayWindow::onSliderMoved);
    connect(_timer, &QTimer::timeout, this, &ReplayWindow::onTimerTick);
    connect(_btnSelectAll, &QPushButton::clicked, this, &ReplayWindow::onSelectAllClicked);
    connect(_btnDeselectAll, &QPushButton::clicked, this, &ReplayWindow::onDeselectAllClicked);
    connect(_filterTree, &QTreeWidget::itemChanged, this, &ReplayWindow::onFilterItemChanged);
    connect(_backend, &Backend::beginMeasurement, this, &ReplayWindow::onBeginMeasurement);
    connect(_backend, &Backend::endMeasurement, this, &ReplayWindow::onEndMeasurement);
}

ReplayWindow::~ReplayWindow()
{
    if (_playing || _paused) {
        _timer->stop();
        _backend->setReplayRunning(false);
    }
}

void ReplayWindow::retranslateUi()
{
    _btnLoad->setText(tr("Load..."));
    _btnPlayPause->setText(_playing ? tr("Pause") : (_paused ? tr("Resume") : tr("Play")));
    _btnStop->setText(tr("Stop"));
    _btnStep->setText(tr("Step +1"));
    _btnLoadToTrace->setText(tr("Load to Trace"));
    _btnLoadToTrace->setToolTip(tr("Import enabled messages directly into Trace and Graph"));
    _btnSplitSave->setText(tr("Split Save..."));
    _btnSplitSave->setToolTip(tr("Split large trace into smaller files by frame count"));
    _btnSelectAll->setText(tr("Select All"));
    _btnDeselectAll->setText(tr("Deselect All"));
    _speedLabel->setText(tr("Speed:"));
    _speedSpin->setToolTip(tr("Playback speed multiplier"));
    _cbAutoplay->setText(tr("Autoplay"));
    _cbAutoplay->setToolTip(tr("Automatically start/stop replay with measurement"));
    _cbLoop->setText(tr("Loop"));
    _cbLoop->setToolTip(tr("Restart replay after all messages were sent"));
    _fileLabel->setPlaceholderText(tr("No trace file loaded"));
    _infoBox->setPlaceholderText(tr("Trace file info..."));
    _filterTree->setHeaderLabels({tr("Interface / CAN ID"), tr("Name"), tr("RX"), tr("TX"), tr("Count"), tr("Output")});
}

void ReplayWindow::onLoadClicked()
{
    const QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Load Trace File"),
        _traceFilePath,
        tr("All Supported (*.asc *.candump *.csv);;"
           "Vector ASC (*.asc);;"
           "Linux candump (*.candump);;"
           "CAN/FD Raw CSV (*.csv);;"
           "All Files (*)"));
    if (filename.isEmpty()) {
        return;
    }
    loadTraceFile(filename);
}

void ReplayWindow::loadTraceFile(const QString &filename)
{
    if (_playing || _paused) {
        onStopClicked();
    }

    TraceReadResult result = TraceFileReader::readFile(filename);
    if (!result.ok || result.messages.isEmpty()) {
        const QString err = result.errorMessage.isEmpty()
            ? tr("Failed to parse trace file or file is empty.")
            : result.errorMessage;
        _infoBox->setPlainText(tr("Error: %1").arg(err));
        log_error(tr("ReplayWindow: failed to load %1: %2").arg(filename, err));
        _messages.clear();
        _messageInterfaces.clear();
        _btnPlayPause->setEnabled(false);
        _btnStop->setEnabled(false);
        _btnStep->setEnabled(false);
        _btnLoadToTrace->setEnabled(false);
        _btnSplitSave->setEnabled(false);
        _slider->setEnabled(false);
        _filterTree->clear();
        _enabledDirs.clear();
        return;
    }

    _messages = std::move(result.messages);
    _messageInterfaces = std::move(result.interfaceNames);
    _playbackIndex = 0;
    _playing = false;
    _paused = false;
    _firstTraceTimestamp = _messages.first().getFloatTimestamp();
    _traceFilePath = filename;
    _fileLabel->setText(filename);

    _slider->setRange(0, static_cast<int>(_messages.size()) - 1);
    _slider->setValue(0);
    _slider->setEnabled(true);
    _btnPlayPause->setEnabled(true);
    _btnPlayPause->setText(tr("Play"));
    _btnStop->setEnabled(false);
    _btnStep->setEnabled(true);
    _btnLoadToTrace->setEnabled(true);
    _btnSplitSave->setEnabled(true);

    const double duration = _messages.last().getFloatTimestamp() - _messages.first().getFloatTimestamp();
    QString info = tr("File: %1\nMessages: %2\nDuration: %3 s")
                       .arg(QFileInfo(filename).fileName())
                       .arg(_messages.size())
                       .arg(duration, 0, 'f', 3);
    if (!result.warnings.isEmpty()) {
        info += QStringLiteral("\n") + tr("Warnings: %1").arg(result.warnings.join(QStringLiteral("; ")));
    }
    _infoBox->setPlainText(info);

    buildFilterTree();
    updatePositionLabel();
}

void ReplayWindow::buildFilterTree()
{
    _filterTree->blockSignals(true);
    _filterTree->clear();
    _enabledDirs.clear();
    _channelCombos.clear();

    struct IdInfo
    {
        int count = 0;
        bool hasRx = false;
        bool hasTx = false;
    };

    QMap<QString, QMap<uint32_t, IdInfo>> ifaceIds;
    for (int i = 0; i < _messages.size(); i++) {
        const QString &iface = _messageInterfaces[i];
        const uint32_t id = _messages[i].isErrorFrame() ? ErrorFrameId : _messages[i].getId();
        IdInfo &info = ifaceIds[iface][id];
        info.count++;
        if (_messages[i].isRX())
            info.hasRx = true;
        else
            info.hasTx = true;
    }

    const BusInterfaceIdList availableInterfaces = _backend->getInterfaceList();

    for (auto ifaceIt = ifaceIds.constBegin(); ifaceIt != ifaceIds.constEnd(); ++ifaceIt) {
        const QString &iface = ifaceIt.key();
        const QMap<uint32_t, IdInfo> &ids = ifaceIt.value();

        auto *ifaceItem = new QTreeWidgetItem(_filterTree);
        ifaceItem->setText(0, tr("%1 (CAN)").arg(iface));
        ifaceItem->setData(0, Qt::UserRole + 1, iface);
        ifaceItem->setFlags(ifaceItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
        ifaceItem->setCheckState(0, Qt::Checked);

        auto *combo = new QComboBox(_filterTree);
        combo->addItem(tr("Trace only"), QVariant(static_cast<int>(-1)));
        for (BusInterfaceId id : availableInterfaces) {
            combo->addItem(_backend->getInterfaceName(id), QVariant(static_cast<int>(id)));
        }
        _channelCombos[iface] = combo;

        int ifaceTotal = 0;
        QMap<uint32_t, uint8_t> enabledMap;

        QList<uint32_t> sortedIds = ids.keys();
        std::sort(sortedIds.begin(), sortedIds.end());

        for (uint32_t id : sortedIds) {
            const IdInfo &info = ids[id];
            ifaceTotal += info.count;

            auto *idItem = new QTreeWidgetItem(ifaceItem);
            if (id == ErrorFrameId) {
                idItem->setText(0, tr("ERROR"));
            } else {
                idItem->setText(0, QStringLiteral("0x%1").arg(id, 0, 16, QChar('0')).toUpper());
                BusMessage tmp;
                tmp.setId(id);
                tmp.setExtended(id > 0x7FF);
                if (CanDbMessage *dbMsg = _backend->findDbMessage(tmp)) {
                    idItem->setText(1, dbMsg->getName());
                }
            }

            idItem->setFlags(idItem->flags() | Qt::ItemIsUserCheckable);
            idItem->setCheckState(0, Qt::Checked);
            uint8_t dirs = 0;
            if (info.hasRx) {
                idItem->setCheckState(2, Qt::Checked);
                dirs |= DirRx;
            }
            if (info.hasTx) {
                idItem->setCheckState(3, Qt::Checked);
                dirs |= DirTx;
            }

            idItem->setText(4, QString::number(info.count));
            idItem->setTextAlignment(4, Qt::AlignRight | Qt::AlignVCenter);
            idItem->setData(0, Qt::UserRole, id);
            idItem->setData(0, Qt::UserRole + 1, iface);

            enabledMap[id] = dirs;
        }

        ifaceItem->setText(4, QString::number(ifaceTotal));
        ifaceItem->setTextAlignment(4, Qt::AlignRight | Qt::AlignVCenter);
        _enabledDirs[iface] = enabledMap;

        _filterTree->setItemWidget(ifaceItem, 5, combo);
    }

    _filterTree->expandAll();
    _filterTree->blockSignals(false);
}

void ReplayWindow::onFilterItemChanged(QTreeWidgetItem *item, int column)
{
    _filterTree->blockSignals(true);

    if (item->childCount() > 0) {
        const Qt::CheckState state = item->checkState(0);
        const QString iface = item->data(0, Qt::UserRole + 1).toString();
        for (int i = 0; i < item->childCount(); i++) {
            item->child(i)->setCheckState(0, state);
        }
        if (state == Qt::Checked) {
            for (int i = 0; i < item->childCount(); i++) {
                auto *child = item->child(i);
                const uint32_t id = child->data(0, Qt::UserRole).toUInt();
                uint8_t dirs = 0;
                if (child->data(2, Qt::CheckStateRole).isValid() && child->checkState(2) == Qt::Checked)
                    dirs |= DirRx;
                if (child->data(3, Qt::CheckStateRole).isValid() && child->checkState(3) == Qt::Checked)
                    dirs |= DirTx;
                _enabledDirs[iface][id] = dirs;
            }
        } else {
            _enabledDirs[iface].clear();
        }
    } else {
        const QString iface = item->data(0, Qt::UserRole + 1).toString();
        const uint32_t id = item->data(0, Qt::UserRole).toUInt();

        if (item->checkState(0) == Qt::Checked) {
            uint8_t dirs = 0;
            if (item->data(2, Qt::CheckStateRole).isValid() && item->checkState(2) == Qt::Checked)
                dirs |= DirRx;
            if (item->data(3, Qt::CheckStateRole).isValid() && item->checkState(3) == Qt::Checked)
                dirs |= DirTx;
            _enabledDirs[iface][id] = dirs;
        } else if (column == 0) {
            _enabledDirs[iface].remove(id);
        } else {
            uint8_t dirs = 0;
            if (item->data(2, Qt::CheckStateRole).isValid() && item->checkState(2) == Qt::Checked)
                dirs |= DirRx;
            if (item->data(3, Qt::CheckStateRole).isValid() && item->checkState(3) == Qt::Checked)
                dirs |= DirTx;
            _enabledDirs[iface][id] = dirs;
        }

        if (QTreeWidgetItem *parent = item->parent()) {
            int checked = 0, unchecked = 0;
            for (int i = 0; i < parent->childCount(); i++) {
                if (parent->child(i)->checkState(0) == Qt::Checked)
                    checked++;
                else
                    unchecked++;
            }
            if (checked == parent->childCount())
                parent->setCheckState(0, Qt::Checked);
            else if (unchecked == parent->childCount())
                parent->setCheckState(0, Qt::Unchecked);
            else
                parent->setCheckState(0, Qt::PartiallyChecked);
        }
    }

    _filterTree->blockSignals(false);
}

void ReplayWindow::onSelectAllClicked()
{
    _filterTree->blockSignals(true);
    for (int i = 0; i < _filterTree->topLevelItemCount(); i++) {
        QTreeWidgetItem *ifaceItem = _filterTree->topLevelItem(i);
        ifaceItem->setCheckState(0, Qt::Checked);
        const QString iface = ifaceItem->data(0, Qt::UserRole + 1).toString();
        for (int j = 0; j < ifaceItem->childCount(); j++) {
            auto *child = ifaceItem->child(j);
            child->setCheckState(0, Qt::Checked);
            const uint32_t id = child->data(0, Qt::UserRole).toUInt();
            uint8_t dirs = 0;
            if (child->data(2, Qt::CheckStateRole).isValid()) {
                child->setCheckState(2, Qt::Checked);
                dirs |= DirRx;
            }
            if (child->data(3, Qt::CheckStateRole).isValid()) {
                child->setCheckState(3, Qt::Checked);
                dirs |= DirTx;
            }
            _enabledDirs[iface][id] = dirs;
        }
    }
    _filterTree->blockSignals(false);
}

void ReplayWindow::onDeselectAllClicked()
{
    _filterTree->blockSignals(true);
    for (int i = 0; i < _filterTree->topLevelItemCount(); i++) {
        QTreeWidgetItem *ifaceItem = _filterTree->topLevelItem(i);
        ifaceItem->setCheckState(0, Qt::Unchecked);
        const QString iface = ifaceItem->data(0, Qt::UserRole + 1).toString();
        _enabledDirs[iface].clear();
        for (int j = 0; j < ifaceItem->childCount(); j++) {
            auto *child = ifaceItem->child(j);
            child->setCheckState(0, Qt::Unchecked);
            if (child->data(2, Qt::CheckStateRole).isValid())
                child->setCheckState(2, Qt::Unchecked);
            if (child->data(3, Qt::CheckStateRole).isValid())
                child->setCheckState(3, Qt::Unchecked);
        }
    }
    _filterTree->blockSignals(false);
}

bool ReplayWindow::isMessageEnabled(int index) const
{
    const QString &iface = _messageInterfaces[index];
    const uint32_t id = _messages[index].isErrorFrame() ? ErrorFrameId : _messages[index].getId();
    auto ifaceIt = _enabledDirs.constFind(iface);
    if (ifaceIt == _enabledDirs.constEnd())
        return false;
    auto idIt = ifaceIt->constFind(id);
    if (idIt == ifaceIt->constEnd())
        return false;
    const uint8_t flag = _messages[index].isRX() ? DirRx : DirTx;
    return (*idIt & flag) != 0;
}

BusInterfaceId ReplayWindow::getMappedInterface(const QString &channel) const
{
    auto it = _channelCombos.constFind(channel);
    if (it == _channelCombos.constEnd())
        return InvalidBusInterfaceId;
    const int val = it.value()->currentData().toInt();
    return val >= 0 ? static_cast<BusInterfaceId>(val) : InvalidBusInterfaceId;
}

void ReplayWindow::onPlayPauseClicked()
{
    if (_messages.isEmpty()) {
        return;
    }

    if (_playing) {
        // Pause: preserve _playbackIndex so Resume continues smoothly
        _playing = false;
        _paused = true;
        _timer->stop();
        _btnPlayPause->setText(tr("Resume"));
        _btnStep->setEnabled(true);
        return;
    }

    if (_playbackIndex >= _messages.size()) {
        _playbackIndex = 0;
        _slider->setValue(0);
    }

    const bool freshStart = !_paused;
    _playing = true;
    _paused = false;
    _btnPlayPause->setText(tr("Pause"));
    _btnStop->setEnabled(true);
    _btnStep->setEnabled(false);
    _btnLoad->setEnabled(false);
    _slider->setEnabled(true);

    if (freshStart) {
        _wallAnchorTime = _backend->currentTimeStamp();
        _firstTraceTimestamp = _messages[0].getFloatTimestamp();
        _backend->setReplayRunning(true);
    }

    _traceStartTime = _messages[_playbackIndex].getFloatTimestamp();
    _elapsed.start();
    _timer->start(1);
}

void ReplayWindow::onStopClicked()
{
    const bool wasActive = _playing || _paused;
    _playing = false;
    _paused = false;
    _timer->stop();
    _playbackIndex = 0;
    _slider->setValue(0);
    _btnPlayPause->setText(tr("Play"));
    _btnPlayPause->setEnabled(!_messages.isEmpty());
    _btnStop->setEnabled(false);
    _btnStep->setEnabled(!_messages.isEmpty());
    _btnLoad->setEnabled(true);
    _slider->setEnabled(!_messages.isEmpty());
    updatePositionLabel();
    if (wasActive) {
        _backend->setReplayRunning(false);
    }
}

void ReplayWindow::onStepClicked()
{
    if (_messages.isEmpty() || _playing) {
        return;
    }
    if (_playbackIndex >= _messages.size()) {
        _playbackIndex = 0;
    }
    if (!_paused) {
        _wallAnchorTime = _backend->currentTimeStamp();
        _firstTraceTimestamp = _messages[0].getFloatTimestamp();
    }
    while (_playbackIndex < _messages.size()) {
        const int idx = _playbackIndex++;
        if (isMessageEnabled(idx)) {
            dispatchSingleMessage(idx, false);
            break;
        }
    }
    _slider->setValue(qMin(_playbackIndex, static_cast<int>(_messages.size()) - 1));
    updatePositionLabel();
}

void ReplayWindow::onLoadToTraceClicked()
{
    if (_messages.isEmpty()) {
        return;
    }

    QVector<BusMessage> filtered;
    filtered.reserve(_messages.size());
    for (int i = 0; i < _messages.size(); ++i) {
        if (!isMessageEnabled(i)) {
            continue;
        }
        BusMessage msg = _messages[i];
        const BusInterfaceId mapped = getMappedInterface(_messageInterfaces[i]);
        if (mapped != InvalidBusInterfaceId) {
            msg.setInterfaceId(mapped);
        }
        filtered.append(msg);
    }

    if (filtered.isEmpty()) {
        QMessageBox::information(this, tr("Load to Trace"), tr("No enabled messages match the current filter."));
        return;
    }

    BusTrace *trace = _backend->getTrace();
    const int limit = trace->maxSize();
    if (filtered.size() > limit) {
        QMessageBox box(this);
        box.setIcon(QMessageBox::Warning);
        box.setWindowTitle(tr("Large Trace File"));
        box.setText(tr("Selected trace contains %1 messages, exceeding the in-memory limit (%2).")
                        .arg(filtered.size())
                        .arg(limit));
        box.setInformativeText(tr("You can split it into smaller files, or load the first %1 messages into Trace.").arg(limit));
        QPushButton *splitBtn = box.addButton(tr("Split & Save..."), QMessageBox::AcceptRole);
        QPushButton *truncBtn = box.addButton(tr("Load First %1").arg(limit), QMessageBox::ActionRole);
        box.addButton(QMessageBox::Cancel);
        box.exec();

        if (box.clickedButton() == splitBtn) {
            onSplitSaveClicked();
            return;
        }
        if (box.clickedButton() != truncBtn) {
            return;
        }
    }

    trace->loadMessages(filtered, true);
    log_info(tr("Loaded %1 replay messages into Trace.").arg(qMin(static_cast<int>(filtered.size()), limit)));
}

void ReplayWindow::onSplitSaveClicked()
{
    if (_messages.isEmpty()) {
        return;
    }

    bool ok = false;
    const int defaultChunk = qMax(1000, _backend->getTrace()->maxSize());
    const int framesPerFile = QInputDialog::getInt(
        this,
        tr("Split Trace by Frame Count"),
        tr("Maximum frames per output file:"),
        defaultChunk,
        100,
        5000000,
        10000,
        &ok);
    if (!ok) {
        return;
    }

    const QString basePath = QFileDialog::getSaveFileName(
        this,
        tr("Save Split Trace Files"),
        _traceFilePath,
        tr("CAN/FD Raw CSV (*.csv);;"
           "Vector ASC (*.asc);;"
           "Linux candump (*.candump)"));
    if (basePath.isEmpty()) {
        return;
    }

    const auto fmt = traceFormatFromPath(basePath).value_or(TraceFileFormat::Csv);
    const QFileInfo fi(basePath);
    TraceReadResult data;
    data.ok = true;
    data.messages = _messages;
    data.interfaceNames = _messageInterfaces;
    QStringList written;
    QString err;
    const bool splitOk = TraceFileReader::writeSplitFiles(
        data,
        fi.absolutePath(),
        fi.completeBaseName(),
        fmt,
        framesPerFile,
        &written,
        &err);

    if (!splitOk || written.isEmpty()) {
        QMessageBox::critical(this, tr("Split Save Failed"), err.isEmpty() ? tr("No files written.") : err);
        return;
    }

    QMessageBox::information(this,
                             tr("Split Save Complete"),
                             tr("Saved %1 split file(s) starting at:\n%2").arg(written.size()).arg(written.first()));
}

void ReplayWindow::onBeginMeasurement()
{
    if (_cbAutoplay->isChecked() && !_messages.isEmpty() && !_playing) {
        _playbackIndex = 0;
        _paused = false;
        _slider->setValue(0);
        onPlayPauseClicked();
    }
}

void ReplayWindow::onEndMeasurement()
{
    if (_cbAutoplay->isChecked() && (_playing || _paused)) {
        onStopClicked();
    }
}

void ReplayWindow::onSliderMoved(int value)
{
    _playbackIndex = qBound(0, value, static_cast<int>(_messages.size()) - 1);
    if (_playing && !_messages.isEmpty()) {
        // Re-anchor playback clock immediately so seeking during Play works seamlessly
        _traceStartTime = _messages[_playbackIndex].getFloatTimestamp();
        _elapsed.restart();
    }
    updatePositionLabel();
}

void ReplayWindow::dispatchSingleMessage(int index, bool moreToFollow)
{
    BusTrace *trace = _backend->getTrace();
    BusMessage msg = _messages[index];

    const QString &channel = _messageInterfaces[index];
    const int mappedInt = _channelCombos.contains(channel) ? _channelCombos[channel]->currentData().toInt() : -1;
    BusInterface *intf = (mappedInt >= 0) ? _backend->getInterfaceById(static_cast<BusInterfaceId>(mappedInt)) : nullptr;

    if (intf && intf->isOpen() && !msg.isErrorFrame()) {
        msg.setTimestamp(_backend->currentTimeStamp());
        msg.setInterfaceId(static_cast<BusInterfaceId>(mappedInt));
        msg.setRX(false);
        intf->sendMessage(msg);
        return;
    }

    // Trace-only mode: preserve exact original inter-frame delta-t anchored at playback start
    const double relSec = msg.getFloatTimestamp() - _firstTraceTimestamp;
    msg.setTimestamp(_wallAnchorTime + relSec);
    if (mappedInt >= 0) {
        msg.setInterfaceId(static_cast<BusInterfaceId>(mappedInt));
    }
    trace->enqueueMessage(msg, moreToFollow);
}

void ReplayWindow::onTimerTick()
{
    if (!_playing || _playbackIndex >= _messages.size()) {
        onStopClicked();
        return;
    }

    const double speed = _speedSpin->value();
    const double wallElapsed = _elapsed.nsecsElapsed() / 1.0e9;
    const double traceDeadline = _traceStartTime + wallElapsed * speed;

    while (_playbackIndex < _messages.size()) {
        const double msgTime = _messages[_playbackIndex].getFloatTimestamp();
        if (msgTime > traceDeadline) {
            break;
        }

        if (isMessageEnabled(_playbackIndex)) {
            bool moreToFollow = false;
            for (int next = _playbackIndex + 1; next < _messages.size(); next++) {
                if (_messages[next].getFloatTimestamp() > traceDeadline) {
                    break;
                }
                if (isMessageEnabled(next)) {
                    moreToFollow = true;
                    break;
                }
            }
            dispatchSingleMessage(_playbackIndex, moreToFollow);
        }
        _playbackIndex++;
    }

    _slider->blockSignals(true);
    _slider->setValue(qMin(_playbackIndex, static_cast<int>(_messages.size()) - 1));
    _slider->blockSignals(false);
    updatePositionLabel();

    if (_playbackIndex >= _messages.size()) {
        if (_cbLoop->isChecked()) {
            _playbackIndex = 0;
            _slider->setValue(0);
            _traceStartTime = _messages[0].getFloatTimestamp();
            _wallAnchorTime = _backend->currentTimeStamp();
            _elapsed.restart();
        } else {
            onStopClicked();
        }
    }
}

QString ReplayWindow::formatTimeMmSs(double sec) const
{
    if (sec < 0.0 || !std::isfinite(sec)) {
        sec = 0.0;
    }
    const qint64 totalMs = static_cast<qint64>(std::llround(sec * 1000.0));
    const qint64 mins = totalMs / 60000;
    const qint64 secs = (totalMs % 60000) / 1000;
    const qint64 ms = totalMs % 1000;
    return QStringLiteral("%1:%2.%3")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0'))
        .arg(ms, 3, 10, QLatin1Char('0'));
}

void ReplayWindow::updatePositionLabel()
{
    double curSec = 0.0;
    double totalSec = 0.0;
    if (!_messages.isEmpty()) {
        const double t0 = _messages.first().getFloatTimestamp();
        totalSec = _messages.last().getFloatTimestamp() - t0;
        const int idx = qBound(0, _playbackIndex, static_cast<int>(_messages.size()) - 1);
        curSec = _messages[idx].getFloatTimestamp() - t0;
    }
    _posLabel->setText(QStringLiteral("%1 / %2 (%3 / %4)")
                           .arg(formatTimeMmSs(curSec), formatTimeMmSs(totalSec))
                           .arg(_playbackIndex)
                           .arg(_messages.size()));
    const int pct = _messages.isEmpty() ? 0 : static_cast<int>(_playbackIndex * 100 / _messages.size());
    _progressBar->setValue(pct);
}

bool ReplayWindow::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    (void)backend;
    root.setAttribute(QStringLiteral("file"), _traceFilePath);
    root.setAttribute(QStringLiteral("autoplay"), _cbAutoplay->isChecked() ? QStringLiteral("1") : QStringLiteral("0"));
    root.setAttribute(QStringLiteral("loop"), _cbLoop->isChecked() ? QStringLiteral("1") : QStringLiteral("0"));
    root.setAttribute(QStringLiteral("speed"), QString::number(_speedSpin->value()));

    for (auto ifaceIt = _enabledDirs.constBegin(); ifaceIt != _enabledDirs.constEnd(); ++ifaceIt) {
        QDomElement ifaceEl = xml.createElement(QStringLiteral("filter"));
        ifaceEl.setAttribute(QStringLiteral("channel"), ifaceIt.key());

        if (_channelCombos.contains(ifaceIt.key())) {
            QComboBox *combo = _channelCombos[ifaceIt.key()];
            ifaceEl.setAttribute(QStringLiteral("mappedInterface"), combo->currentText());
        }

        const QMap<uint32_t, uint8_t> &ids = ifaceIt.value();
        for (auto idIt = ids.constBegin(); idIt != ids.constEnd(); ++idIt) {
            QDomElement idEl = xml.createElement(QStringLiteral("id"));
            idEl.setAttribute(QStringLiteral("value"), QString::number(idIt.key()));
            idEl.setAttribute(QStringLiteral("dirs"), QString::number(idIt.value()));
            ifaceEl.appendChild(idEl);
        }

        root.appendChild(ifaceEl);
    }
    return true;
}

bool ReplayWindow::loadXML(Backend &backend, QDomElement &el)
{
    (void)backend;
    _cbAutoplay->setChecked(el.attribute(QStringLiteral("autoplay"), QStringLiteral("0")) == QStringLiteral("1"));
    _cbLoop->setChecked(el.attribute(QStringLiteral("loop"), QStringLiteral("0")) == QStringLiteral("1"));
    _speedSpin->setValue(el.attribute(QStringLiteral("speed"), QStringLiteral("1.0")).toDouble());

    const QString filepath = el.attribute(QStringLiteral("file"));
    if (!filepath.isEmpty()) {
        loadTraceFile(filepath);
    }

    QDomNodeList filters = el.elementsByTagName(QStringLiteral("filter"));
    for (int i = 0; i < filters.size(); i++) {
        QDomElement filterEl = filters.at(i).toElement();
        const QString channel = filterEl.attribute(QStringLiteral("channel"));

        if (_channelCombos.contains(channel)) {
            const QString mappedName = filterEl.attribute(QStringLiteral("mappedInterface"));
            QComboBox *combo = _channelCombos[channel];
            const int idx = combo->findText(mappedName);
            if (idx >= 0) {
                combo->setCurrentIndex(idx);
            }
        }

        if (_enabledDirs.contains(channel)) {
            QDomNodeList idNodes = filterEl.elementsByTagName(QStringLiteral("id"));
            for (int j = 0; j < idNodes.size(); j++) {
                QDomElement idEl = idNodes.at(j).toElement();
                const uint32_t id = idEl.attribute(QStringLiteral("value")).toUInt();
                const uint8_t dirs = static_cast<uint8_t>(idEl.attribute(QStringLiteral("dirs")).toUInt());
                if (_enabledDirs[channel].contains(id)) {
                    _enabledDirs[channel][id] = dirs;
                }
            }

            for (int t = 0; t < _filterTree->topLevelItemCount(); t++) {
                QTreeWidgetItem *ifaceItem = _filterTree->topLevelItem(t);
                if (ifaceItem->data(0, Qt::UserRole + 1).toString() != channel) {
                    continue;
                }
                for (int c = 0; c < ifaceItem->childCount(); c++) {
                    QTreeWidgetItem *idItem = ifaceItem->child(c);
                    const uint32_t id = idItem->data(0, Qt::UserRole).toUInt();
                    if (!_enabledDirs[channel].contains(id)) {
                        continue;
                    }
                    const uint8_t dirs = _enabledDirs[channel][id];
                    _filterTree->blockSignals(true);
                    if (idItem->checkState(2) != Qt::Unchecked)
                        idItem->setCheckState(2, (dirs & DirRx) ? Qt::Checked : Qt::Unchecked);
                    if (idItem->checkState(3) != Qt::Unchecked)
                        idItem->setCheckState(3, (dirs & DirTx) ? Qt::Checked : Qt::Unchecked);
                    const bool anyEnabled = (dirs & (DirRx | DirTx)) != 0;
                    idItem->setCheckState(0, anyEnabled ? Qt::Checked : Qt::Unchecked);
                    _filterTree->blockSignals(false);
                }
            }
        }
    }
    return true;
}
