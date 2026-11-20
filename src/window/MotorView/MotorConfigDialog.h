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

#pragma once

#include <QDialog>

#include "decoders/motor/MotorTypes.h"

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QGroupBox;
class QLabel;
class QLineEdit;

/// Modal editor for a single MotorConfig.
///
/// Name and CAN ID are validated in accept(); an ID that is unusual for the
/// selected model asks for confirmation instead of being rejected, so custom
/// IDs stay possible. Duplicate IDs need the other motors and are checked by
/// MotorView.
class MotorConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorConfigDialog(const MotorConfig &initial, QWidget *parent = nullptr);

    void accept() override;

    [[nodiscard]] MotorConfig config() const;

private:
    void updateModelHint();
    [[nodiscard]] MotorModel selectedModel() const;

    QLineEdit *_nameEdit = nullptr;
    QComboBox *_modelCombo = nullptr;
    QLineEdit *_idEdit = nullptr;
    QLabel *_idHint = nullptr;
    QGroupBox *_damiaoGroup = nullptr;
    QDoubleSpinBox *_pmaxSpin = nullptr;
    QDoubleSpinBox *_vmaxSpin = nullptr;
    QDoubleSpinBox *_tmaxSpin = nullptr;
    QCheckBox *_invertCheck = nullptr;
    QCheckBox *_enabledCheck = nullptr;
    quint32 _fallbackId = 0x201; ///< config() result while the ID field is invalid
};
