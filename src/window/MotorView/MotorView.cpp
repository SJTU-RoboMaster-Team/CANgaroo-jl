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

#include "MotorView.h"

#include <QAbstractItemView>
#include <QBrush>
#include <QDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QRegularExpression>
#include <QTableWidget>
#include <QVBoxLayout>

#include "core/Backend.h"
#include "core/Log.h"
#include "core/MotorManager.h"
#include "core/ThemeManager.h"
#include "decoders/motor/MotorDbc.h"
#include "MotorConfigDialog.h"

namespace
{

/// Accepts "0x201", "201" and "513"; every form is parsed as hexadecimal.
bool parseCanIdText(const QString &text, quint32 *canId)
{
    QString value = text.trimmed();
    if (value.startsWith(QLatin1String("0x"), Qt::CaseInsensitive)) {
        value.remove(0, 2);
    }

    static const QRegularExpression hex11Bit(QStringLiteral("^[0-9a-fA-F]{1,3}$"));
    if (!hex11Bit.match(value).hasMatch()) {
        return false;
    }

    bool ok = false;
    const quint32 id = value.toUInt(&ok, 16);
    if (!ok || id > 0x7FF) {
        return false;
    }
    if (canId) {
        *canId = id;
    }
    return true;
}

} // namespace

MotorView::MotorView(QWidget *parent, Backend &backend)
    : ConfigurableWidget(parent)
    , _backend(backend)
    , _manager(backend.getMotorManager())
{
    setObjectName(QStringLiteral("MotorView"));

    _btnAdd = new QPushButton(this);
    _btnEdit = new QPushButton(this);
    _btnRemove = new QPushButton(this);
    _btnClear = new QPushButton(this);
    _btnExport = new QPushButton(this);

    auto *toolbar = new QHBoxLayout;
    toolbar->addWidget(_btnAdd);
    toolbar->addWidget(_btnEdit);
    toolbar->addWidget(_btnRemove);
    toolbar->addSpacing(12);
    toolbar->addWidget(_btnClear);
    toolbar->addSpacing(12);
    toolbar->addWidget(_btnExport);
    toolbar->addStretch(1);

    _table = new QTableWidget(0, column_count, this);
    _table->setAlternatingRowColors(true);
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table->setSelectionMode(QAbstractItemView::SingleSelection);
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table->setWordWrap(false);
    _table->verticalHeader()->setVisible(false);
    _table->horizontalHeader()->setHighlightSections(false);
    _table->horizontalHeader()->setSectionResizeMode(column_name, QHeaderView::Stretch);
    for (int column = column_model; column < column_count; ++column) {
        _table->horizontalHeader()->setSectionResizeMode(column, QHeaderView::ResizeToContents);
    }

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(toolbar);
    layout->addWidget(_table, 1);

    connect(_btnAdd, &QPushButton::clicked, this, &MotorView::onAddMotor);
    connect(_btnEdit, &QPushButton::clicked, this, &MotorView::onEditMotor);
    connect(_btnRemove, &QPushButton::clicked, this, &MotorView::onRemoveMotor);
    connect(_btnClear, &QPushButton::clicked, this, &MotorView::onClearValues);
    connect(_btnExport, &QPushButton::clicked, this, &MotorView::onExportDbc);
    connect(_table, &QTableWidget::itemSelectionChanged, this, &MotorView::onSelectionChanged);
    connect(_table, &QTableWidget::cellDoubleClicked, this, &MotorView::onCellDoubleClicked);

    if (_manager) {
        connect(_manager, &MotorManager::statesUpdated, this, &MotorView::onStatesUpdated);
        connect(_manager, &MotorManager::configChanged, this, &MotorView::rebuildTable);
    }

    // Item foreground colours are palette based, so they must be re-read when
    // the theme changes even if no new motor data arrives.
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this,
            [this](ThemeManager::Theme) { updateRows(); });

    rebuildTable();
    retranslateUi();
}

bool MotorView::saveXML(Backend &backend, QDomDocument &xml, QDomElement &root)
{
    if (!ConfigurableWidget::saveXML(backend, xml, root)) {
        return false;
    }
    if (!_manager) {
        return true;
    }

    const QVector<MotorConfig> motors = _manager->motors();
    for (const MotorConfig &motor : motors) {
        QDomElement motorEl = xml.createElement(QStringLiteral("motor"));
        motorEl.setAttribute(QStringLiteral("name"), motor.name);
        motorEl.setAttribute(QStringLiteral("model"), MotorProtocols::modelKey(motor.model));
        motorEl.setAttribute(QStringLiteral("feedback_id"), formatCanId(motor.feedbackId));
        motorEl.setAttribute(QStringLiteral("invert"),
                             motor.invert ? QStringLiteral("1") : QStringLiteral("0"));
        motorEl.setAttribute(QStringLiteral("enabled"),
                             motor.enabled ? QStringLiteral("1") : QStringLiteral("0"));
        if (MotorProtocols::isDamiaoModel(motor.model)) {
            motorEl.setAttribute(QStringLiteral("damiao_pmax"),
                                 QString::number(motor.damiaoPmax, 'f', 3));
            motorEl.setAttribute(QStringLiteral("damiao_vmax"),
                                 QString::number(motor.damiaoVmax, 'f', 3));
            motorEl.setAttribute(QStringLiteral("damiao_tmax"),
                                 QString::number(motor.damiaoTmax, 'f', 3));
        }
        root.appendChild(motorEl);
    }
    return true;
}

bool MotorView::loadXML(Backend &backend, QDomElement &el)
{
    if (!ConfigurableWidget::loadXML(backend, el)) {
        return false;
    }

    QVector<MotorConfig> motors;
    for (QDomElement motorEl = el.firstChildElement(QStringLiteral("motor"));
         !motorEl.isNull();
         motorEl = motorEl.nextSiblingElement(QStringLiteral("motor"))) {
        MotorConfig motor;
        motor.name = motorEl.attribute(QStringLiteral("name"));

        const QString modelKey = motorEl.attribute(QStringLiteral("model"));
        bool modelOk = false;
        motor.model = MotorProtocols::modelFromKey(modelKey, &modelOk);
        if (!modelOk) {
            log_warning(tr("MotorView: unknown motor model \"%1\" in workspace, using %2.")
                            .arg(modelKey, MotorProtocols::modelKey(motor.model)));
        }

        quint32 feedbackId = 0;
        const QString idText = motorEl.attribute(QStringLiteral("feedback_id"));
        if (parseCanIdText(idText, &feedbackId)) {
            motor.feedbackId = feedbackId;
        } else {
            log_warning(tr("MotorView: invalid feedback CAN ID \"%1\" for motor \"%2\", keeping %3.")
                            .arg(idText, motor.name, formatCanId(motor.feedbackId)));
        }

        motor.invert = motorEl.attribute(QStringLiteral("invert"), QStringLiteral("0"))
            == QLatin1String("1");
        motor.enabled = motorEl.attribute(QStringLiteral("enabled"), QStringLiteral("1"))
            == QLatin1String("1");
        if (MotorProtocols::isDamiaoModel(motor.model)) {
            // Workspaces written before the MIT limit registers existed keep
            // the struct defaults (12.5 rad / 30 rad/s / 10 N·m).
            bool ok = false;
            const double pmax = motorEl.attribute(QStringLiteral("damiao_pmax")).toDouble(&ok);
            if (ok) {
                motor.damiaoPmax = pmax;
            }
            const double vmax = motorEl.attribute(QStringLiteral("damiao_vmax")).toDouble(&ok);
            if (ok) {
                motor.damiaoVmax = vmax;
            }
            const double tmax = motorEl.attribute(QStringLiteral("damiao_tmax")).toDouble(&ok);
            if (ok) {
                motor.damiaoTmax = tmax;
            }
        }
        motors.append(motor);
    }

    if (_manager) {
        // Emits configChanged(), which rebuilds the table.
        _manager->setMotors(motors);
    }
    return true;
}

void MotorView::retranslateUi()
{
    if (!_table || !_btnAdd) {
        return; // changeEvent() can arrive before the UI exists
    }

    _table->setHorizontalHeaderLabels({
        tr("Name"),
        tr("Model"),
        tr("CAN ID"),
        tr("Angle (°)"),
        tr("Speed (rpm)"),
        tr("Current (A)"),
        tr("Torque (N·m)"),
        tr("Temp (°C)"),
        tr("Status"),
    });

    _btnAdd->setText(tr("Add Motor..."));
    _btnAdd->setToolTip(tr("Add a motor to decode its feedback frames"));
    _btnEdit->setText(tr("Edit..."));
    _btnEdit->setToolTip(tr("Edit the selected motor"));
    _btnRemove->setText(tr("Remove"));
    _btnRemove->setToolTip(tr("Remove the selected motor"));
    _btnClear->setText(tr("Clear Values"));
    _btnClear->setToolTip(tr("Reset the decoded values without removing the motors"));
    _btnExport->setText(tr("Export DBC..."));
    _btnExport->setToolTip(tr("Save the auto-generated database for these motors as a .dbc file"));

    if (!_manager) {
        return;
    }

    const QVector<MotorConfig> motors = _manager->motors();
    if (_table->rowCount() != static_cast<int>(motors.size())) {
        rebuildTable(); // creates the rows with the translated model names
        return;
    }
    for (int row = 0; row < _table->rowCount(); ++row) {
        applyConfigRow(row, motors.at(row));
    }
    updateRows(); // re-applies the status text in the new language
}

void MotorView::onStatesUpdated()
{
    updateRows();
}

void MotorView::onSelectionChanged()
{
    updateButtons();
}

void MotorView::onAddMotor()
{
    if (!_manager) {
        return;
    }

    MotorConfig config;
    config.feedbackId = nextFreeDjiFeedbackId();
    if (!runConfigDialog(config, -1)) {
        return;
    }

    QVector<MotorConfig> motors = _manager->motors();
    motors.append(config);
    _manager->setMotors(motors); // rebuilds the table through configChanged()
    if (_table->rowCount() > 0) {
        _table->selectRow(_table->rowCount() - 1);
    }
}

void MotorView::onEditMotor()
{
    if (!_manager) {
        return;
    }

    const int row = selectedRow();
    QVector<MotorConfig> motors = _manager->motors();
    if (row < 0 || row >= static_cast<int>(motors.size())) {
        return;
    }

    MotorConfig config = motors.at(row);
    if (!runConfigDialog(config, row)) {
        return;
    }

    motors[row] = config;
    _manager->setMotors(motors); // rebuilds the table through configChanged()
    if (row < _table->rowCount()) {
        _table->selectRow(row);
    }
}

void MotorView::onRemoveMotor()
{
    if (!_manager) {
        return;
    }

    const int row = selectedRow();
    QVector<MotorConfig> motors = _manager->motors();
    if (row < 0 || row >= static_cast<int>(motors.size())) {
        return;
    }

    QString name = motors.at(row).name;
    if (name.isEmpty()) {
        name = formatCanId(motors.at(row).feedbackId);
    }
    const auto answer = QMessageBox::question(this, tr("Remove Motor"),
                                              tr("Remove motor \"%1\"?").arg(name),
                                              QMessageBox::Yes | QMessageBox::No,
                                              QMessageBox::No);
    if (answer != QMessageBox::Yes) {
        return;
    }

    motors.removeAt(row);
    _manager->setMotors(motors); // rebuilds the table through configChanged()
    if (!motors.isEmpty()) {
        _table->selectRow(qMin(row, static_cast<int>(motors.size()) - 1));
    }
}

void MotorView::onClearValues()
{
    if (_manager) {
        _manager->resetStates(); // emits statesUpdated()
    }
}

void MotorView::onExportDbc()
{
    if (!_manager) {
        return;
    }

    QString error;
    const QString dbcText = MotorDbc::buildText(_manager->motors(), &error);
    if (!error.isEmpty()) {
        QMessageBox::warning(this, tr("Export DBC"), error);
        return;
    }
    if (dbcText.isEmpty()) {
        QMessageBox::information(this, tr("Export DBC"),
                                 tr("Configure at least one motor first."));
        return;
    }

    const QString path = QFileDialog::getSaveFileName(
        this, tr("Export DBC"), QStringLiteral("motors.dbc"), tr("DBC Files (*.dbc)"));
    if (path.isEmpty()) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Export DBC"),
                              tr("Cannot open %1 for writing.").arg(path));
        return;
    }
    // DbcParser reads DBC files as Latin-1 (the usual DBC encoding). Writing
    // UTF-8 would corrupt non-ASCII units such as the middle dot in "N·m" when
    // the exported file is loaded again.
    const QByteArray bytes = dbcText.toLatin1();
    if (file.write(bytes) != qint64(bytes.size())) {
        QMessageBox::critical(this, tr("Export DBC"), tr("Cannot write %1.").arg(path));
    }
}

void MotorView::onCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0) {
        return;
    }
    _table->selectRow(row);
    onEditMotor();
}

void MotorView::rebuildTable()
{
    if (!_table) {
        return;
    }

    const int previousRow = selectedRow();
    const QVector<MotorConfig> motors = _manager ? _manager->motors() : QVector<MotorConfig>();
    const int rowCount = static_cast<int>(motors.size());

    _table->setRowCount(rowCount);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < column_count; ++column) {
            if (_table->item(row, column)) {
                continue;
            }
            auto *item = new QTableWidgetItem;
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            const bool numeric = column == column_angle || column == column_speed
                || column == column_current || column == column_torque
                || column == column_temperature;
            item->setTextAlignment(numeric ? (Qt::AlignRight | Qt::AlignVCenter)
                                           : (Qt::AlignLeft | Qt::AlignVCenter));
            _table->setItem(row, column, item);
        }
        applyConfigRow(row, motors.at(row));
    }

    updateRows();
    if (previousRow >= 0 && previousRow < rowCount) {
        _table->selectRow(previousRow);
    }
    updateButtons();
}

void MotorView::updateRows()
{
    if (!_table || !_manager) {
        return;
    }

    const QVector<MotorConfig> motors = _manager->motors();
    const int rowCount = static_cast<int>(motors.size());
    if (_table->rowCount() != rowCount) {
        rebuildTable();
        return;
    }

    const QVector<MotorState> states = _manager->states();
    const int stateCount = static_cast<int>(states.size());
    const QString noData = QStringLiteral("—");
    for (int row = 0; row < rowCount; ++row) {
        const MotorConfig &motor = motors.at(row);
        const MotorState state = row < stateCount ? states.at(row) : MotorState();
        const bool hasData = state.hasData;
        const bool damiao = MotorProtocols::isDamiaoModel(motor.model);
        // A row can show live values only while the motor is enabled and its
        // feedback frame is not timed out; everything else is dimmed.
        const bool dimmed = !(motor.enabled && hasData && state.online);

        setCellValue(row, column_angle,
                     hasData ? QString::number(state.angleDeg, 'f', 1) : noData);
        setCellValue(row, column_speed,
                     hasData ? QString::number(qRound(state.speedRpm)) : noData);

        // C610/M2006 and DAMIAO do not document a physical current
        // conversion; show the raw field instead of inventing amps.
        QString currentText = noData;
        QString currentTip;
        if (hasData && state.currentValid) {
            currentText = QString::number(state.currentA, 'f', 2);
        } else if (hasData) {
            currentTip = tr("raw torque: %1").arg(state.rawCurrent);
        }
        setCellValue(row, column_current, currentText);
        setCellToolTip(row, column_current, currentTip);

        // Only DAMIAO feedback frames carry a torque value.
        setCellValue(row, column_torque,
                     hasData && damiao ? QString::number(state.torqueNm, 'f', 2) : noData);

        QString temperatureText = noData;
        QString temperatureTip;
        if (hasData) {
            if (state.temperatureValid) {
                temperatureText = QString::number(qRound(state.temperatureC));
            }
            if (damiao) {
                temperatureTip = tr("MOS: %1 °C / Rotor: %2 °C")
                                     .arg(QString::number(state.mosTemperatureC, 'f', 1),
                                          QString::number(state.temperatureC, 'f', 1));
            }
        }
        setCellValue(row, column_temperature, temperatureText);
        setCellToolTip(row, column_temperature, temperatureTip);

        QString status = noData;
        if (hasData) {
            status = state.online ? tr("Online") : tr("Offline");
        }
        setCellValue(row, column_status, status);

        setRowDimmed(row, dimmed);
    }
}

void MotorView::updateButtons()
{
    const int row = selectedRow();
    const bool hasManager = _manager != nullptr;
    _btnAdd->setEnabled(hasManager);
    _btnEdit->setEnabled(hasManager && row >= 0);
    _btnRemove->setEnabled(hasManager && row >= 0);
    _btnClear->setEnabled(hasManager && _manager->hasMotors());
}

void MotorView::applyConfigRow(int row, const MotorConfig &config)
{
    if (auto *item = _table->item(row, column_name)) {
        item->setText(config.name);
    }
    if (auto *item = _table->item(row, column_model)) {
        item->setText(modelDisplayName(config.model));
    }
    if (auto *item = _table->item(row, column_canId)) {
        item->setText(formatCanId(config.feedbackId));
    }
}

void MotorView::setCellValue(int row, int column, const QString &text)
{
    if (QTableWidgetItem *item = _table->item(row, column)) {
        if (item->text() != text) {
            item->setText(text);
        }
    }
}

void MotorView::setCellToolTip(int row, int column, const QString &text)
{
    if (QTableWidgetItem *item = _table->item(row, column)) {
        if (item->toolTip() != text) {
            item->setToolTip(text);
        }
    }
}

void MotorView::setRowDimmed(int row, bool dimmed)
{
    const QColor color = dimmed ? palette().color(QPalette::Disabled, QPalette::Text)
                                : palette().color(QPalette::Text);
    const QBrush brush(color);
    for (int column = 0; column < column_count; ++column) {
        if (QTableWidgetItem *item = _table->item(row, column)) {
            item->setForeground(brush);
        }
    }
}

bool MotorView::runConfigDialog(MotorConfig &config, int replaceIndex)
{
    if (!_manager) {
        return false;
    }

    MotorConfig candidate = config;
    for (;;) {
        MotorConfigDialog dialog(candidate, this);
        if (dialog.exec() != QDialog::Accepted) {
            return false;
        }
        candidate = dialog.config();

        const int conflict = findFeedbackIdConflict(candidate.feedbackId, replaceIndex);
        if (conflict < 0) {
            break;
        }

        const QVector<MotorConfig> motors = _manager->motors();
        QString otherName = motors.at(conflict).name;
        if (otherName.isEmpty()) {
            otherName = formatCanId(motors.at(conflict).feedbackId);
        }
        QMessageBox::warning(
            this, tr("Motor Configuration"),
            tr("CAN ID %1 is already used by motor \"%2\". Choose a different ID.")
                .arg(formatCanId(candidate.feedbackId), otherName));
    }

    config = candidate;
    return true;
}

int MotorView::findFeedbackIdConflict(quint32 feedbackId, int exceptIndex) const
{
    if (!_manager) {
        return -1;
    }
    const QVector<MotorConfig> motors = _manager->motors();
    for (int row = 0; row < static_cast<int>(motors.size()); ++row) {
        if (row != exceptIndex && motors.at(row).feedbackId == feedbackId) {
            return row;
        }
    }
    return -1;
}

quint32 MotorView::nextFreeDjiFeedbackId() const
{
    for (quint32 id = 0x201; id <= 0x20B; ++id) {
        if (findFeedbackIdConflict(id, -1) < 0) {
            return id;
        }
    }
    return 0x201;
}

int MotorView::selectedRow() const
{
    if (!_table || !_table->selectionModel()) {
        return -1;
    }
    const QModelIndexList rows = _table->selectionModel()->selectedRows();
    if (rows.isEmpty()) {
        return -1;
    }
    return rows.first().row();
}

QString MotorView::modelDisplayName(MotorModel model) const
{
    switch (model) {
    case MotorModel::DjiM3508:
        return tr("DJI M3508");
    case MotorModel::DjiM2006:
        return tr("DJI M2006");
    case MotorModel::DjiGM6020:
        return tr("DJI GM6020");
    case MotorModel::DamiaoMit:
        return tr("Damiao DM (MIT)");
    }
    return QString();
}

QString MotorView::formatCanId(quint32 canId)
{
    return QStringLiteral("0x")
        + QString::number(canId, 16).toUpper().rightJustified(3, QLatin1Char('0'));
}
