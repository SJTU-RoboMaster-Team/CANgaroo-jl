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

#include "MotorConfigDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>

namespace
{

/// Formats an 11 bit CAN ID like "0x201".
QString formatCanId(quint32 canId)
{
    return QStringLiteral("0x")
        + QString::number(canId, 16).toUpper().rightJustified(3, QLatin1Char('0'));
}

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

MotorConfigDialog::MotorConfigDialog(const MotorConfig &initial, QWidget *parent)
    : QDialog(parent)
    , _fallbackId(initial.feedbackId)
{
    setWindowTitle(tr("Motor Configuration"));

    _nameEdit = new QLineEdit(initial.name, this);
    _nameEdit->setPlaceholderText(tr("e.g. Front Left"));

    _modelCombo = new QComboBox(this);
    _modelCombo->addItem(tr("DJI M3508"), static_cast<int>(MotorModel::DjiM3508));
    _modelCombo->addItem(tr("DJI M2006"), static_cast<int>(MotorModel::DjiM2006));
    _modelCombo->addItem(tr("DJI GM6020"), static_cast<int>(MotorModel::DjiGM6020));
    _modelCombo->addItem(tr("Damiao DM (MIT)"), static_cast<int>(MotorModel::DamiaoMit));
    int modelIndex = _modelCombo->findData(static_cast<int>(initial.model));
    if (modelIndex < 0) {
        modelIndex = 0;
    }
    _modelCombo->setCurrentIndex(modelIndex);

    _idEdit = new QLineEdit(formatCanId(initial.feedbackId), this);
    _idEdit->setPlaceholderText(QStringLiteral("0x201"));
    _idHint = new QLabel(this);
    _idHint->setWordWrap(true);
    _idHint->setTextFormat(Qt::PlainText);

    auto *idColumn = new QVBoxLayout;
    idColumn->setContentsMargins(0, 0, 0, 0);
    idColumn->addWidget(_idEdit);
    idColumn->addWidget(_idHint);

    // DAMIAO MIT output limits are writable registers on the motor, so they
    // belong to the motor configuration; DJI models do not have them.
    _damiaoGroup = new QGroupBox(tr("Damiao MIT limits"), this);
    _pmaxSpin = new QDoubleSpinBox(_damiaoGroup);
    _pmaxSpin->setRange(0.1, 20.0);
    _pmaxSpin->setDecimals(3);
    _pmaxSpin->setSingleStep(0.1);
    _pmaxSpin->setSuffix(QStringLiteral(" rad"));
    _pmaxSpin->setValue(initial.damiaoPmax);

    _vmaxSpin = new QDoubleSpinBox(_damiaoGroup);
    _vmaxSpin->setRange(1.0, 100.0);
    _vmaxSpin->setDecimals(3);
    _vmaxSpin->setSingleStep(1.0);
    _vmaxSpin->setSuffix(QStringLiteral(" rad/s"));
    _vmaxSpin->setValue(initial.damiaoVmax);

    _tmaxSpin = new QDoubleSpinBox(_damiaoGroup);
    _tmaxSpin->setRange(0.1, 100.0);
    _tmaxSpin->setDecimals(3);
    _tmaxSpin->setSingleStep(0.5);
    _tmaxSpin->setSuffix(QStringLiteral(" N·m"));
    _tmaxSpin->setValue(initial.damiaoTmax);

    auto *damiaoForm = new QFormLayout(_damiaoGroup);
    damiaoForm->addRow(tr("Pmax (rad):"), _pmaxSpin);
    damiaoForm->addRow(tr("Vmax (rad/s):"), _vmaxSpin);
    damiaoForm->addRow(tr("Tmax (N·m):"), _tmaxSpin);

    _invertCheck = new QCheckBox(tr("Invert direction"), this);
    _invertCheck->setChecked(initial.invert);
    _enabledCheck = new QCheckBox(tr("Enabled"), this);
    _enabledCheck->setChecked(initial.enabled);

    auto *form = new QFormLayout;
    form->addRow(tr("Name:"), _nameEdit);
    form->addRow(tr("Model:"), _modelCombo);
    form->addRow(tr("Feedback CAN ID:"), idColumn);
    form->addRow(_damiaoGroup);
    form->addRow(_invertCheck);
    form->addRow(_enabledCheck);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &MotorConfigDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &MotorConfigDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);

    connect(_modelCombo, &QComboBox::currentIndexChanged, this, &MotorConfigDialog::updateModelHint);
    updateModelHint();
    _nameEdit->setFocus();
}

MotorConfig MotorConfigDialog::config() const
{
    MotorConfig config;
    config.name = _nameEdit->text().trimmed();
    config.model = selectedModel();
    quint32 feedbackId = 0;
    config.feedbackId = parseCanIdText(_idEdit->text(), &feedbackId) ? feedbackId : _fallbackId;
    config.damiaoPmax = _pmaxSpin->value();
    config.damiaoVmax = _vmaxSpin->value();
    config.damiaoTmax = _tmaxSpin->value();
    config.invert = _invertCheck->isChecked();
    config.enabled = _enabledCheck->isChecked();
    return config;
}

MotorModel MotorConfigDialog::selectedModel() const
{
    return static_cast<MotorModel>(_modelCombo->currentData().toInt());
}

void MotorConfigDialog::accept()
{
    if (_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Motor Configuration"), tr("Please enter a motor name."));
        _nameEdit->setFocus();
        return;
    }

    quint32 feedbackId = 0;
    if (!parseCanIdText(_idEdit->text(), &feedbackId)) {
        QMessageBox::warning(this, tr("Motor Configuration"),
                             tr("Please enter a valid 11 bit CAN ID, e.g. 0x201."));
        _idEdit->setFocus();
        _idEdit->selectAll();
        return;
    }

    const MotorModel model = selectedModel();
    if (MotorProtocols::isDjiModel(model)) {
        // Recommended feedback IDs: 0x201-0x208 for the C620/C610 ESCs,
        // 0x205-0x20B for the GM6020. Custom IDs are allowed after a warning.
        quint32 recommendedMin = 0x201;
        quint32 recommendedMax = 0x208;
        if (model == MotorModel::DjiGM6020) {
            recommendedMin = 0x205;
            recommendedMax = 0x20B;
        }
        if (feedbackId < recommendedMin || feedbackId > recommendedMax) {
            const auto answer = QMessageBox::warning(
                this, tr("Motor Configuration"),
                tr("CAN ID %1 is outside the usual feedback range %2-%3 for the selected model. Use it anyway?")
                    .arg(formatCanId(feedbackId),
                         formatCanId(recommendedMin),
                         formatCanId(recommendedMax)),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (answer != QMessageBox::Yes) {
                _idEdit->setFocus();
                _idEdit->selectAll();
                return;
            }
        }
    }

    QDialog::accept();
}

void MotorConfigDialog::updateModelHint()
{
    const MotorModel model = selectedModel();
    const bool damiao = MotorProtocols::isDamiaoModel(model);
    _damiaoGroup->setVisible(damiao);

    QString hint;
    if (damiao) {
        hint = tr("Enter the motor CAN ID (Master ID / feedback frame ID), 0x000-0x7FF.");
    } else if (model == MotorModel::DjiGM6020) {
        hint = tr("DJI feedback frame ID, usually 0x205-0x20B (0x200 + ESC ID).");
    } else {
        hint = tr("DJI feedback frame ID, usually 0x201-0x208 (0x200 + ESC ID).");
    }
    _idHint->setText(hint);
    _idEdit->setToolTip(hint);
}
