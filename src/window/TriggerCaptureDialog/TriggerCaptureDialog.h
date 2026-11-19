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
#include "core/TriggerCaptureManager.h"

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableWidget;

class TriggerCaptureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TriggerCaptureDialog(const TriggerCaptureConfig &config,
                                  bool currentlyArmed,
                                  QWidget *parent = nullptr);

    [[nodiscard]] TriggerCaptureConfig config() const;
    [[nodiscard]] bool shouldArm() const;

    static TriggerRule makeQuickFrameRule(uint32_t canId);
    static TriggerRule makeQuickWatchdogRule(uint32_t canId, int timeoutMs = 200);

private slots:
    void browseFolder();
    void addRule();
    void removeSelectedRule();
    void updateRuleSummaryRow(int row);

private:
    QLineEdit *_folderEdit = nullptr;
    QLineEdit *_prefixEdit = nullptr;
    QComboBox *_formatCombo = nullptr;
    QDoubleSpinBox *_preSecSpin = nullptr;
    QDoubleSpinBox *_postSecSpin = nullptr;
    QSpinBox *_maxPreFramesSpin = nullptr;
    QSpinBox *_holdoffSpin = nullptr;
    QCheckBox *_oneShotCheck = nullptr;
    QCheckBox *_armCheck = nullptr;

    QTableWidget *_ruleTable = nullptr;
    QVector<TriggerRule> _rules;

    void populateRuleTable();
    void appendRuleRow(const TriggerRule &rule);
    QString ruleKindLabel(TriggerRule::Kind kind) const;
    QString ruleDetailSummary(const TriggerRule &rule) const;
    bool editRuleDialog(TriggerRule &rule);
};
