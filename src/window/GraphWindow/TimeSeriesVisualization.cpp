/*

  Copyright (c) 2026 Jayachandran Dharuman

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

#include "TimeSeriesVisualization.h"
#include <QVBoxLayout>
#include <float.h>
#include <QToolTip>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include "core/ThemeManager.h"

TimeSeriesVisualization::TimeSeriesVisualization(QWidget *parent, Backend &backend)
    : VisualizationWidget(parent, backend), _windowDuration(0), _autoScroll(true), _isUpdatingRange(false)
{
    _updateTimer = new QTimer(this);
    connect(_updateTimer, &QTimer::timeout, this, &TimeSeriesVisualization::onActivated);
    _updateTimer->start(30);
    _chart = new QChart();
    _chart->legend()->setVisible(true);
    _chart->legend()->setAlignment(Qt::AlignBottom);
    _chart->setTitle(tr("Time Series Graph"));

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);
    _chartView->setRubberBand(QChartView::HorizontalRubberBand);
    _chartView->setMouseTracking(true);
    _chartView->viewport()->setMouseTracking(true); // Ensure viewport tracks mouse
    _chartView->viewport()->installEventFilter(this); // Catch events on viewport
    _chartView->installEventFilter(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_chartView);
    layout->setContentsMargins(0, 0, 0, 0);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText(tr("Time [s]"));
    _chart->addAxis(axisX, Qt::AlignBottom);
    connect(axisX, &QValueAxis::rangeChanged, this, &TimeSeriesVisualization::updateYAxisRange);
    connect(axisX, &QValueAxis::rangeChanged, this, &TimeSeriesVisualization::onAxisRangeChanged);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(tr("Value"));
    _chart->addAxis(axisY, Qt::AlignLeft);
    
    // Create cursor line
    _cursorLine = new QGraphicsLineItem(_chart);
    QPen cursorPen(palette().color(QPalette::WindowText), 1, Qt::DashLine);
    _cursorLine->setPen(cursorPen);
    _cursorLine->setZValue(1000); // High Z-value to be on top
    _cursorLine->hide();

    // Dual measurement cursors A and B
    const ThemeColors &tc = ThemeManager::instance().colors();
    _cursorALine = new QGraphicsLineItem(_chart);
    _cursorALine->setPen(QPen(tc.cursorA, 2, Qt::DashLine));
    _cursorALine->setZValue(1100);
    _cursorALine->hide();

    _cursorBLine = new QGraphicsLineItem(_chart);
    _cursorBLine->setPen(QPen(tc.cursorB, 2, Qt::DashLine));
    _cursorBLine->setZValue(1100);
    _cursorBLine->hide();

    // Create Tooltip Box
    _tooltipBox = new QGraphicsRectItem(_chart);
    _tooltipBox->setBrush(QBrush(palette().color(QPalette::ToolTipBase)));
    _tooltipBox->setPen(QPen(palette().color(QPalette::ToolTipText), 1));
    _tooltipBox->setZValue(2000); // Very high Z-value
    _tooltipBox->hide();

    _tooltipText = new QGraphicsTextItem(_tooltipBox);
    _tooltipText->setDefaultTextColor(palette().color(QPalette::ToolTipText));
    _tooltipText->setTextInteractionFlags(Qt::NoTextInteraction);
    _tooltipText->setZValue(2001);

    // Add shadow effect to tooltip box
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 80));
    _tooltipBox->setGraphicsEffect(shadow);

    setMouseTracking(true);
    applyTheme(ThemeManager::instance().currentTheme());
}

TimeSeriesVisualization::~TimeSeriesVisualization()
{
}

void TimeSeriesVisualization::retranslateUi()
{
    _chart->setTitle(tr("Time Series Graph"));
    for (QAbstractAxis *axis : _chart->axes())
    {
        if (axis->orientation() == Qt::Horizontal)
            axis->setTitleText(tr("Time [s]"));
        else
            axis->setTitleText(tr("Value"));
    }
}

void TimeSeriesVisualization::addDecodedData(const QMap<GraphSignal*, DecodedSignalData>& newPoints)
{
    for (auto it = newPoints.begin(); it != newPoints.end(); ++it) {
        GraphSignal* signal = it.key();
        if (!_signals.contains(signal)) continue;

        const DecodedSignalData &data = it.value();
        
        _signalBuffers[signal].timestamps.append(data.timestamps);
        _signalBuffers[signal].values.append(data.values);
        _signalBusMap[signal] = data.interfaceId;

        if (_signalBuffers[signal].timestamps.size() > 210000) {
            _signalBuffers[signal].timestamps.remove(0, 10000);
            _signalBuffers[signal].values.remove(0, 10000);
            _syncIndices[signal] = qMax(0, _syncIndices.value(signal, 0) - 10000);
        }
    }
}

void TimeSeriesVisualization::addMessage(const BusMessage &msg)
{
    // Keeping for backwards compatibility if needed directly
    double timestamp = msg.getFloatTimestamp();

    if (_startTime < 0) {
        setGlobalStartTime(timestamp);
    }

    double t = timestamp - _startTime;
    BusInterfaceId msgIfId = msg.getInterfaceId();

    for (GraphSignal *signal : _signals) {
        if (signal->isPresentInMessage(msg)) {
            // Network Context Filtering:
            if (_signalInterfaces.contains(signal)) {
                if (!_signalInterfaces[signal].contains(msgIfId)) {
                    continue;
                }
            }

            double value = signal->extractPhysicalFromMessage(msg);
            
            _signalBuffers[signal].timestamps.append(t);
            _signalBuffers[signal].values.append(value);
            _signalBusMap[signal] = msg.getInterfaceId();

            if (_signalBuffers[signal].timestamps.size() > 210000) {
                _signalBuffers[signal].timestamps.remove(0, 10000);
                _signalBuffers[signal].values.remove(0, 10000);
                _syncIndices[signal] = qMax(0, _syncIndices.value(signal, 0) - 10000);
            }
        }
    }
}

void TimeSeriesVisualization::onActivated()
{
    // Find latest timestamp across all signal buffers for scrolling
    double latestT = 0;
    for (const auto &buffer : _signalBuffers.values()) {
        if (!buffer.timestamps.isEmpty()) {
            latestT = qMax(latestT, buffer.timestamps.last());
        }
    }

    // Synchronize series with buffers (Batch Update)
    for (auto it = _seriesMap.begin(); it != _seriesMap.end(); ++it) {
        GraphSignal *sig = it.key();
        QLineSeries *series = it.value();
        const auto &buffer = _signalBuffers[sig];
        int syncIdx = _syncIndices.value(sig, 0);

        if (syncIdx < buffer.timestamps.size()) {
            QList<QPointF> points;
            points.reserve(buffer.timestamps.size() - syncIdx);
            for (int i = syncIdx; i < buffer.timestamps.size(); ++i) {
                points.append(QPointF(buffer.timestamps[i], buffer.values[i]));
            }
            series->append(points);
            _syncIndices[sig] = buffer.timestamps.size();
        }

        // Keep series size in sync with buffer block pruning
        if (series->count() > 200000) {
            series->removePoints(0, series->count() - 190000);
        }
    }
    
    if (!_autoScroll || _chart->axes(Qt::Horizontal).isEmpty()) return;
    
    double t = latestT;

    _isUpdatingRange = true;
    QAbstractAxis *axisX = _chart->axes(Qt::Horizontal).first();
    if (_windowDuration > 0) {
        double windowSize = (double)_windowDuration;
        if (t > windowSize) {
            axisX->setRange(t - windowSize, t);
        } else {
            axisX->setRange(0, windowSize);
        }
    } else {
        // "All" Mode: Show from 0 to current timestamp
        axisX->setRange(0, qMax(10.0, t));
    }
    _isUpdatingRange = false;

    updateYAxisRange();
    updateOverlayLines();
}

void TimeSeriesVisualization::setSignalColor(GraphSignal *signal, const QColor &color)
{
    VisualizationWidget::setSignalColor(signal, color);
    if (_seriesMap.contains(signal)) {
        _seriesMap[signal]->setColor(color);
    }
    if (_tracers.contains(signal)) {
        _tracers[signal]->setBrush(color);
    }
}

void TimeSeriesVisualization::updateYAxisRange()
{
    if (_chart->axes(Qt::Vertical).isEmpty() || _seriesMap.isEmpty()) return;

    double minY = DBL_MAX;
    double maxY = -DBL_MAX;
    bool hasData = false;

    QValueAxis *axisX = qobject_cast<QValueAxis*>(_chart->axes(Qt::Horizontal).first());
    double minX = axisX->min();
    double maxX = axisX->max();

    for (auto it = _signalBuffers.begin(); it != _signalBuffers.end(); ++it) {
        const auto &buffer = it.value();
        if (buffer.timestamps.isEmpty()) continue;

        // Binary search to find the first point in the visible window
        auto startIt = std::lower_bound(buffer.timestamps.begin(), buffer.timestamps.end(), minX);
        int startIdx = std::distance(buffer.timestamps.begin(), startIt);

        for (int i = startIdx; i < buffer.timestamps.size(); ++i) {
            double tx = buffer.timestamps[i];
            if (tx > maxX) break; // Optimization: Stop once we exceed visible range
            
            double val = buffer.values[i];
            minY = qMin(minY, val);
            maxY = qMax(maxY, val);
            hasData = true;
        }
    }

    if (hasData) {
        double range = maxY - minY;
        if (range < 0.0001) { // Very small range
            minY -= 0.5;
            maxY += 0.5;
        } else {
            // Apply 10% padding
            minY -= range * 0.1;
            maxY += range * 0.1;
        }
        
        // Stabilize axis by rounding to 4 decimal places to prevent flickering ticks
        minY = std::floor(minY * 10000.0) / 10000.0;
        maxY = std::ceil(maxY * 10000.0) / 10000.0;

        _chart->axes(Qt::Vertical).first()->setRange(minY, maxY);
    }
}

void TimeSeriesVisualization::clear()
{
    for (auto series : _seriesMap.values()) {
        series->clear();
    }
    for (auto it = _signalBuffers.begin(); it != _signalBuffers.end(); ++it) {
        it.value().timestamps.clear();
        it.value().values.clear();
    }
    for (auto it = _syncIndices.begin(); it != _syncIndices.end(); ++it) {
        *it = 0;
    }
    _startTime = -1;

    if (!_chart->axes(Qt::Horizontal).isEmpty()) {
        _chart->axes(Qt::Horizontal).first()->setRange(0, 5); // Fallback range
    }
    if (!_chart->axes(Qt::Vertical).isEmpty()) {
        _chart->axes(Qt::Vertical).first()->setRange(-10, 10);
    }
    _autoScroll = true;
    clearCursors();
    for (auto &tm : _triggerMarkers) {
        delete tm.line;
    }
    _triggerMarkers.clear();
}

void TimeSeriesVisualization::clearSignals()
{
    for (auto series : _seriesMap.values()) {
        _chart->removeSeries(series);
        delete series;
    }
    for (auto tracer : _tracers.values()) {
        delete tracer;
    }
    _tracers.clear();
    _seriesMap.clear();
    _signals.clear();
    _signalBusMap.clear();
    _signalBuffers.clear();
    _syncIndices.clear();
    _startTime = -1;

    if (!_chart->axes(Qt::Horizontal).isEmpty()) {
        _chart->axes(Qt::Horizontal).first()->setRange(0, 5);
    }
    if (!_chart->axes(Qt::Vertical).isEmpty()) {
        _chart->axes(Qt::Vertical).first()->setRange(-10, 10);
    }
    _autoScroll = true;
    clearCursors();
    for (auto &tm : _triggerMarkers) {
        delete tm.line;
    }
    _triggerMarkers.clear();
}

void TimeSeriesVisualization::wheelEvent(QWheelEvent *event)
{
    _autoScroll = false;
    
    QValueAxis *axisX = qobject_cast<QValueAxis*>(_chart->axes(Qt::Horizontal).first());
    if (axisX) {
        double min = axisX->min();
        double max = axisX->max();
        double center = min + (max - min) * 0.5;
        double factor = (event->angleDelta().y() > 0) ? 0.8 : 1.2;
        
        double newRange = (max - min) * factor;
        _isUpdatingRange = true;
        axisX->setRange(center - newRange * 0.5, center + newRange * 0.5);
        _isUpdatingRange = false;
    }
    
    updateYAxisRange();
    event->accept();
}

bool TimeSeriesVisualization::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == _chartView || watched == _chartView->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            emit mouseMoved(static_cast<QMouseEvent*>(event));
        } else if (_cursorsEnabled && event->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent*>(event);
            const QPointF scenePos = _chartView->mapToScene(me->pos());
            const QPointF chartPos = _chart->mapFromScene(scenePos);
            if (_chart->plotArea().contains(chartPos)) {
                const double t = _chart->mapToValue(chartPos).x();
                if (me->button() == Qt::LeftButton && !(me->modifiers() & Qt::ShiftModifier)) {
                    _cursorAT = t;
                    _hasCursorA = true;
                    updateOverlayLines();
                    emitCursorSummary();
                    return true;
                }
                if (me->button() == Qt::RightButton
                    || (me->button() == Qt::LeftButton && (me->modifiers() & Qt::ShiftModifier))) {
                    _cursorBT = t;
                    _hasCursorB = true;
                    updateOverlayLines();
                    emitCursorSummary();
                    return true;
                }
            }
        }
    }
    return VisualizationWidget::eventFilter(watched, event);
}

void TimeSeriesVisualization::zoomIn()
{
    _autoScroll = false;
    QValueAxis *axisX = qobject_cast<QValueAxis*>(_chart->axes(Qt::Horizontal).first());
    if (axisX) {
        double min = axisX->min();
        double max = axisX->max();
        double newRange = (max - min) * 0.8;
        double center = min + (max - min) * 0.5;
        _isUpdatingRange = true;
        axisX->setRange(center - newRange * 0.5, center + newRange * 0.5);
        _isUpdatingRange = false;
    }
    updateYAxisRange();
}

void TimeSeriesVisualization::zoomOut()
{
    _autoScroll = false;
    QValueAxis *axisX = qobject_cast<QValueAxis*>(_chart->axes(Qt::Horizontal).first());
    if (axisX) {
        double min = axisX->min();
        double max = axisX->max();
        double newRange = (max - min) * 1.2;
        double center = min + (max - min) * 0.5;
        _isUpdatingRange = true;
        axisX->setRange(center - newRange * 0.5, center + newRange * 0.5);
        _isUpdatingRange = false;
    }
    updateYAxisRange();
}

void TimeSeriesVisualization::resetZoom()
{
    _autoScroll = true;
    onActivated();
}

void TimeSeriesVisualization::setWindowDuration(int seconds)
{
    _windowDuration = seconds;
    _autoScroll = true;
}

void TimeSeriesVisualization::addSignal(GraphSignal *signal, const BusInterfaceIdList &interfaces)
{
    if (_seriesMap.contains(signal)) return;

    VisualizationWidget::addSignal(signal, interfaces);

    QLineSeries *series = new QLineSeries();
    series->setName(signal->name());
    _chart->addSeries(series);
    
    if (!_chart->axes(Qt::Horizontal).isEmpty()) {
        series->attachAxis(_chart->axes(Qt::Horizontal).first());
    }
    if (!_chart->axes(Qt::Vertical).isEmpty()) {
        series->attachAxis(_chart->axes(Qt::Vertical).first());
    }

    _seriesMap[signal] = series;

    QGraphicsEllipseItem *tracer = new QGraphicsEllipseItem(-4, -4, 8, 8, _chart);
    tracer->setBrush(series->color());
    tracer->setPen(QPen(Qt::white, 1));
    tracer->setZValue(1500);
    tracer->hide();
    _tracers[signal] = tracer;

    // Fix visibility bug: Force auto-scroll so the axis immediately tracks to the new live data point
    _autoScroll = true;
}

void TimeSeriesVisualization::onAxisRangeChanged(qreal min, qreal max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    if (!_isUpdatingRange) {
        _autoScroll = false;
    }
    updateOverlayLines();
}

void TimeSeriesVisualization::setCursorsEnabled(bool enabled)
{
    _cursorsEnabled = enabled;
    _chartView->setRubberBand(enabled ? QChartView::NoRubberBand : QChartView::HorizontalRubberBand);
    if (!enabled) {
        clearCursors();
    } else {
        emitCursorSummary();
    }
}

void TimeSeriesVisualization::clearCursors()
{
    _hasCursorA = false;
    _hasCursorB = false;
    if (_cursorALine) _cursorALine->hide();
    if (_cursorBLine) _cursorBLine->hide();
    emit cursorsUpdated(QString());
}

void TimeSeriesVisualization::addTriggerMarker(double relSec, const QString &label)
{
    const ThemeColors &tc = ThemeManager::instance().colors();
    auto *line = new QGraphicsLineItem(_chart);
    line->setPen(QPen(tc.triggerMarker, 2, Qt::DashDotLine));
    line->setZValue(1050);
    line->setToolTip(label);
    _triggerMarkers.append({relSec, label, line});
    updateOverlayLines();
}

void TimeSeriesVisualization::updateOverlayLines()
{
    const QRectF area = _chart->plotArea();
    if (area.width() <= 0 || area.height() <= 0 || _chart->axes(Qt::Horizontal).isEmpty()) {
        return;
    }

    auto positionVerticalLine = [&](QGraphicsLineItem *line, double t, bool active) {
        if (!line) return;
        if (!active) {
            line->hide();
            return;
        }
        const QPointF p = _chart->mapToPosition(QPointF(t, 0.0));
        if (p.x() < area.left() || p.x() > area.right()) {
            line->hide();
            return;
        }
        line->setLine(p.x(), area.top(), p.x(), area.bottom());
        line->show();
    };

    positionVerticalLine(_cursorALine, _cursorAT, _cursorsEnabled && _hasCursorA);
    positionVerticalLine(_cursorBLine, _cursorBT, _cursorsEnabled && _hasCursorB);
    for (auto &tm : _triggerMarkers) {
        positionVerticalLine(tm.line, tm.t, true);
    }
}

bool TimeSeriesVisualization::sampleSignalAt(QLineSeries *series, double t, double &outY) const
{
    if (!series) return false;
    const QList<QPointF> pts = series->points();
    if (pts.isEmpty()) return false;
    int left = 0, right = static_cast<int>(pts.size()) - 1;
    while (left < right) {
        const int mid = (left + right) / 2;
        if (pts[mid].x() < t) left = mid + 1;
        else right = mid;
    }
    const int idx = (left > 0 && qAbs(pts[left - 1].x() - t) < qAbs(pts[left].x() - t)) ? left - 1 : left;
    outY = pts[idx].y();
    return true;
}

void TimeSeriesVisualization::emitCursorSummary()
{
    if (!_cursorsEnabled) {
        emit cursorsUpdated(QString());
        return;
    }
    if (!_hasCursorA && !_hasCursorB) {
        emit cursorsUpdated(tr("Cursors: Left-click = A, Right-click / Shift+Click = B"));
        return;
    }

    QStringList parts;
    if (_hasCursorA) parts << QStringLiteral("A: %1 s").arg(_cursorAT, 0, 'f', 4);
    if (_hasCursorB) parts << QStringLiteral("B: %1 s").arg(_cursorBT, 0, 'f', 4);
    if (_hasCursorA && _hasCursorB) {
        const double dt = _cursorBT - _cursorAT;
        parts << QStringLiteral("Δt = %1 s").arg(dt, 0, 'f', 4);
        if (qAbs(dt) > 1e-6) {
            parts << QStringLiteral("1/|Δt| = %1 Hz").arg(1.0 / qAbs(dt), 0, 'f', 2);
        }
        if (!_seriesMap.isEmpty()) {
            auto it = _seriesMap.constBegin();
            double yA = 0.0, yB = 0.0;
            if (sampleSignalAt(it.value(), _cursorAT, yA) && sampleSignalAt(it.value(), _cursorBT, yB)) {
                parts << QStringLiteral("%1: yA=%2, yB=%3, Δy=%4")
                             .arg(it.key()->name())
                             .arg(yA, 0, 'f', 3)
                             .arg(yB, 0, 'f', 3)
                             .arg(yB - yA, 0, 'f', 3);
            }
        }
    }
    emit cursorsUpdated(parts.join(QStringLiteral("   |   ")));
}

void TimeSeriesVisualization::setActive(bool active)
{
    if (active) {
        if (!_updateTimer->isActive()) _updateTimer->start(30);
    } else {
        _updateTimer->stop();
    }
}

void TimeSeriesVisualization::applyTheme(ThemeManager::Theme theme)
{
    const ThemeColors& colors = ThemeManager::instance().colors();
    
    _chart->setBackgroundBrush(colors.graphBackground);
    _chart->setTitleBrush(colors.windowText);
    _chart->legend()->setLabelColor(colors.windowText);

    for (auto axis : _chart->axes()) {
        axis->setLabelsColor(colors.graphAxisText);
        axis->setTitleBrush(colors.graphAxisText);
        if (auto vAxis = qobject_cast<QValueAxis*>(axis)) {
            vAxis->setGridLineColor(colors.graphGrid);
        }
    }

    _cursorLine->setPen(QPen(colors.graphCursor, 1, Qt::DashLine));
    if (_cursorALine) _cursorALine->setPen(QPen(colors.cursorA, 2, Qt::DashLine));
    if (_cursorBLine) _cursorBLine->setPen(QPen(colors.cursorB, 2, Qt::DashLine));
    for (auto &tm : _triggerMarkers) {
        if (tm.line) tm.line->setPen(QPen(colors.triggerMarker, 2, Qt::DashDotLine));
    }
    _tooltipBox->setBrush(QBrush(colors.toolTipBase));
    _tooltipBox->setPen(QPen(colors.toolTipText, 1));
    _tooltipText->setDefaultTextColor(colors.toolTipText);

    if (theme == ThemeManager::Dark) {
        _chart->setTheme(QChart::ChartThemeDark);
    } else {
        _chart->setTheme(QChart::ChartThemeLight);
    }
    
    // Explicitly override background again as ChartTheme might change it
    _chart->setBackgroundBrush(colors.graphBackground);
}
