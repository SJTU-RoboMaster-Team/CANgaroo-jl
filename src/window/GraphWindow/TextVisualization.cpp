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

#include "TextVisualization.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGridLayout>
#include <QRegularExpression>
#include <QScrollArea>

#include "helpers/appfonts.h"

TextVisualization::TextVisualization(QWidget *parent, Backend &backend)
    : VisualizationWidget(parent, backend)
{
    _scrollArea = new QScrollArea(this);
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setFrameShape(QFrame::NoFrame);

    _container = new QWidget();
    _containerLayout = new QVBoxLayout(_container);
    _containerLayout->setAlignment(Qt::AlignTop);
    _containerLayout->setSpacing(0); // No spacing between rows for zebra striping
    _containerLayout->setContentsMargins(0, 0, 0, 0);

    _scrollArea->setWidget(_container);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_scrollArea);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    _updateTimer = new QTimer(this);
    connect(_updateTimer, &QTimer::timeout, this, &TextVisualization::updateUi);
    _updateTimer->start(100); // 10Hz throttle
}

TextVisualization::~TextVisualization()
{
}

void TextVisualization::addDecodedData(const QMap<GraphSignal*, DecodedSignalData>& newPoints)
{
    for (auto it = newPoints.begin(); it != newPoints.end(); ++it) {
        GraphSignal* signal = it.key();
        if (!_signals.contains(signal)) continue;

        const DecodedSignalData &data = it.value();
        if (data.timestamps.isEmpty()) continue;
        
        _signalBuffers[signal].timestamps.append(data.timestamps);
        _signalBuffers[signal].values.append(data.values);

        if (_signalDataMap.contains(signal)) {
            _signalDataMap[signal].value = data.values.last();
            _signalDataMap[signal].updated = true;
        }

        if (_signalBuffers[signal].values.size() > 100) {
            int removeCount = _signalBuffers[signal].values.size() - 80;
            _signalBuffers[signal].timestamps.remove(0, removeCount);
            _signalBuffers[signal].values.remove(0, removeCount);
        }
    }
}

void TextVisualization::addMessage(const BusMessage &msg)
{
    BusInterfaceId msgIfId = msg.getInterfaceId();

    for (GraphSignal *signal : _signals) {
        if (signal->isPresentInMessage(msg)) {
            // Network Context Filtering
            if (_signalInterfaces.contains(signal)) {
                if (!_signalInterfaces[signal].contains(msgIfId)) {
                    continue;
                }
            }

            double value = signal->extractPhysicalFromMessage(msg);
            
            // Populate isolated buffer
            _signalBuffers[signal].timestamps.append(msg.getFloatTimestamp());
            _signalBuffers[signal].values.append(value);

            if (_signalDataMap.contains(signal)) {
                _signalDataMap[signal].value = value;
                _signalDataMap[signal].updated = true;
            }
            
            if (_signalBuffers[signal].values.size() > 100) {
                _signalBuffers[signal].timestamps.remove(0);
                _signalBuffers[signal].values.remove(0);
            }
        }
    }
}

void TextVisualization::updateUi()
{
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        if (it.value().updated) {
            it.value().valueLabel->setText(QString::number(it.value().value, 'f', 2));
            it.value().updated = false;
        }
    }
}

void TextVisualization::applyTheme(ThemeManager::Theme theme)
{
    Q_UNUSED(theme);
    const ThemeColors &colors = ThemeManager::instance().colors();
    QColor base = colors.base;
    QColor alternate = colors.alternateBase;
    if (alternate == base) {
        alternate = (base.lightness() > 128) ? base.darker(105) : base.lighter(115);
    }
    QColor border = colors.windowText;
    border.setAlpha(35);

    int index = 0;
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        QFrame *card = qobject_cast<QFrame*>(it.value().card);
        if (!card) continue;

        QColor bg = (index % 2 == 0) ? base : alternate;
        card->setStyleSheet(QString("QFrame { background-color: %1; border-bottom: 1px solid %2; color: %3; }")
                            .arg(bg.name(), border.name(QColor::HexArgb), colors.text.name()));

        if (it.value().valueLabel) {
            it.value().valueLabel->setStyleSheet(
                QString("color: %1;").arg(colors.text.name()));
        }
        auto labels = card->findChildren<QLabel*>();
        for (QLabel *label : labels) {
            if (label == it.value().valueLabel) continue;
            if (label->property("role") == QLatin1String("name")) {
                label->setStyleSheet(QString("color: %1;").arg(colors.text.name()));
            } else if (label->property("role") == QLatin1String("unit")) {
                label->setStyleSheet(QString("color: %1;").arg(colors.graphAxisText.name()));
            }
        }
        index++;
    }
}

void TextVisualization::clear()
{
    _signalBuffers.clear();
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        it.value().value = 0;
        it.value().updated = true;
    }
    updateUi();
}

void TextVisualization::onActivated()
{
    VisualizationWidget::onActivated();
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        it.value().updated = true;
    }
    updateUi();
}

void TextVisualization::clearSignals()
{
    _updateTimer->stop();
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        _containerLayout->removeWidget(it.value().card);
        delete it.value().card;
    }
    _signalDataMap.clear();
    _signals.clear();
    _updateTimer->start();
}

void TextVisualization::addSignal(GraphSignal *signal, const BusInterfaceIdList &interfaces)
{
    if (_signalDataMap.contains(signal)) return;

    VisualizationWidget::addSignal(signal, interfaces);
    createSignalCard(signal);
}

void TextVisualization::createSignalCard(GraphSignal *signal)
{
    QFrame *card = new QFrame(_container);
    card->setFrameStyle(QFrame::NoFrame);
    
    // Zebra striping using palette
    bool isEven = (_signalDataMap.size() % 2 == 0);
    QColor base = palette().color(QPalette::Base);
    QColor alternate = palette().color(QPalette::AlternateBase);
    if (alternate == base) {
        // Fallback if palette doesn't have distinct alternate
        alternate = base.lighter(105);
        if (base.value() > 200) alternate = base.darker(105);
    }
    
    QColor bg = isEven ? base : alternate;
    QColor border = palette().color(QPalette::WindowText);
    border.setAlpha(30);

    card->setStyleSheet(QString("QFrame { background-color: %1; border-bottom: 1px solid %2; }")
                        .arg(bg.name())
                        .arg(border.name()));
    card->setMinimumHeight(50);
    card->setFixedHeight(50); // Locked height for stability

    QHBoxLayout *layout = new QHBoxLayout(card);
    layout->setContentsMargins(15, 0, 15, 0);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignVCenter);

    // 1. Color Indicator
    QLabel *colorLabel = new QLabel(card);
    colorLabel->setFixedSize(12, 12);
    colorLabel->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(getSignalColor(signal).name()));
    layout->addWidget(colorLabel);

    // 2. Signal Name (Expanding)
    QLabel *nameLabel = new QLabel(signal->name(), card);
    nameLabel->setProperty("role", QStringLiteral("name"));
    nameLabel->setFont(AppFonts::uiFont(9.5, QFont::Medium));
    nameLabel->setStyleSheet(QString("color: %1;").arg(ThemeManager::instance().colors().text.name())); 
    nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // Elide text if too long
    QFontMetrics fm(nameLabel->font());
    nameLabel->setText(fm.elidedText(signal->name(), Qt::ElideRight, 200)); 
    layout->addWidget(nameLabel); 

    // Add stretch to push value to the right (dashboard style)
    layout->addStretch();

    // 3. Value (Fixed Width, Monospace)
    QLabel *valueLabel = new QLabel("0.00", card);
    valueLabel->setFixedWidth(120);
    valueLabel->setMinimumWidth(100);
    valueLabel->setFont(AppFonts::monoFont(13.5, QFont::DemiBold));
    valueLabel->setStyleSheet(QString("color: %1;").arg(ThemeManager::instance().colors().text.name())); 
    valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(valueLabel);
    
    // 4. Unit (Fixed Width)
    QLabel *unitLabel = new QLabel(signal->unit(), card);
    unitLabel->setProperty("role", QStringLiteral("unit"));
    unitLabel->setFixedWidth(60);
    unitLabel->setFont(AppFonts::uiFont(9, QFont::Medium));
    unitLabel->setStyleSheet(QString("color: %1;").arg(ThemeManager::instance().colors().graphAxisText.name())); 
    unitLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(unitLabel);

    _containerLayout->addWidget(card);
    
    SignalData data;
    data.value = 0;
    data.updated = false;
    data.valueLabel = valueLabel;
    data.card = card;
    _signalDataMap[signal] = data;
}

void TextVisualization::setSignalColor(GraphSignal *signal, const QColor &color)
{
    VisualizationWidget::setSignalColor(signal, color);
    if (_signalDataMap.contains(signal)) {
        // Update ONLY the indicator icon color
        // Value and Unit labels remain high-contrast Dark Blue/Black for readability
        QFrame *card = qobject_cast<QFrame*>(_signalDataMap[signal].card);
        if (card) {
            auto labels = card->findChildren<QLabel*>();
            if (labels.size() >= 1) {
                // First label is the color icon
                labels[0]->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(color.name()));
            }
        }
    }
}

void TextVisualization::resizeEvent(QResizeEvent *event)
{
    VisualizationWidget::resizeEvent(event);
    
    // Update elided names on resize
    for (auto it = _signalDataMap.begin(); it != _signalDataMap.end(); ++it) {
        GraphSignal *sig = it.key();
        QFrame *card = qobject_cast<QFrame*>(it.value().card);
        if (card) {
            auto labels = card->findChildren<QLabel*>();
            if (labels.size() >= 2) {
                QLabel *nameLabel = labels[1]; // Second label is the name
                 QFontMetrics fm(nameLabel->font());
                 int available = width() - 250; 
                 if (available > 50) {
                     nameLabel->setText(fm.elidedText(sig->name(), Qt::ElideRight, available));
                 }
            }
        }
    }
}

void TextVisualization::updateFontScaling()
{
    // No longer needed with stable row layout
}
