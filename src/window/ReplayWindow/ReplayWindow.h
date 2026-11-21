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
#pragma once

#include "core/ConfigurableWidget.h"
#include "core/Backend.h"
#include <QElapsedTimer>
#include <QMap>

class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QSlider;
class QLabel;
class QTimer;
class QDoubleSpinBox;
class QTreeWidget;
class QTreeWidgetItem;
class QCheckBox;
class QComboBox;
class QProgressBar;

class ReplayWindow : public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit ReplayWindow(QWidget *parent, Backend &backend);
    ~ReplayWindow() override;

    void loadTraceFile(const QString &filename);

    bool saveXML(Backend &backend, QDomDocument &xml, QDomElement &root) override;
    bool loadXML(Backend &backend, QDomElement &el) override;

protected:
    void retranslateUi() override;

private slots:
    void onLoadClicked();
    void onPlayPauseClicked();
    void onStopClicked();
    void onStepClicked();
    void onLoadToTraceClicked();
    void onSplitSaveClicked();
    void onSliderMoved(int value);
    void onTimerTick();
    void onSelectAllClicked();
    void onDeselectAllClicked();
    void onFilterItemChanged(QTreeWidgetItem *item, int column);
    void onBeginMeasurement();
    void onEndMeasurement();

private:
    Backend *_backend;

    QLineEdit *_fileLabel;
    QPushButton *_btnLoad;
    QPushButton *_btnPlayPause;
    QPushButton *_btnStop;
    QPushButton *_btnStep;
    QPushButton *_btnLoadToTrace;
    QPushButton *_btnSplitSave;
    QPushButton *_btnSelectAll;
    QPushButton *_btnDeselectAll;
    QLabel *_speedLabel;
    QDoubleSpinBox *_speedSpin;
    QSlider *_slider;
    QLabel *_posLabel;
    QProgressBar *_progressBar;
    QPlainTextEdit *_infoBox;
    QTreeWidget *_filterTree;
    QCheckBox *_cbAutoplay;
    QCheckBox *_cbLoop;
    QTimer *_timer;

    QString _traceFilePath;
    QVector<BusMessage> _messages;
    QVector<QString> _messageInterfaces;
    int _playbackIndex = 0;
    bool _playing = false;
    bool _paused = false;
    QElapsedTimer _elapsed;
    double _traceStartTime = 0.0;
    double _firstTraceTimestamp = 0.0;
    double _wallAnchorTime = 0.0;

    static constexpr uint8_t DirRx = 0x01;
    static constexpr uint8_t DirTx = 0x02;
    static constexpr uint32_t ErrorFrameId = 0xFFFFFFFF;
    QMap<QString, QMap<uint32_t, uint8_t>> _enabledDirs;
    QMap<QString, QComboBox*> _channelCombos;

    void buildFilterTree();
    void updatePositionLabel();
    void dispatchSingleMessage(int index, bool moreToFollow);
    bool isMessageEnabled(int index) const;
    BusInterfaceId getMappedInterface(const QString &channel) const;
    QString formatTimeMmSs(double sec) const;
};
