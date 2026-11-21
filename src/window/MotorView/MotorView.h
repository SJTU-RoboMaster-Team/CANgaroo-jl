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

#include <QString>

#include "core/ConfigurableWidget.h"
#include "decoders/motor/MotorTypes.h"

class Backend;
class MotorManager;
class QDomDocument;
class QDomElement;
class QPushButton;
class QTableWidget;

/// Table of the configured motors and their latest decoded feedback values.
///
/// Rows map 1:1 to MotorManager::motors(). statesUpdated() is emitted at a
/// bounded rate and only rewrites the value cells; the table is rebuilt from
/// the configuration, never from the refresh path.
class MotorView : public ConfigurableWidget
{
    Q_OBJECT

public:
    explicit MotorView(QWidget *parent, Backend &backend);

    bool saveXML(Backend &backend, QDomDocument &xml, QDomElement &root) override;
    bool loadXML(Backend &backend, QDomElement &el) override;

protected:
    void retranslateUi() override;

private slots:
    void onStatesUpdated();
    void onSelectionChanged();
    void onAddMotor();
    void onEditMotor();
    void onRemoveMotor();
    void onClearValues();
    void onExportDbc();
    void onCellDoubleClicked(int row, int column);

private:
    enum Column
    {
        column_name = 0,
        column_model,
        column_canId,
        column_angle,
        column_speed,
        column_current,
        column_torque,
        column_temperature,
        column_status,
        column_count
    };

    Backend &_backend;
    MotorManager *_manager = nullptr;
    QTableWidget *_table = nullptr;
    QPushButton *_btnAdd = nullptr;
    QPushButton *_btnEdit = nullptr;
    QPushButton *_btnRemove = nullptr;
    QPushButton *_btnClear = nullptr;
    QPushButton *_btnExport = nullptr;

    void rebuildTable();
    void updateRows();
    void updateButtons();
    void applyConfigRow(int row, const MotorConfig &config);
    void setCellValue(int row, int column, const QString &text);
    void setCellToolTip(int row, int column, const QString &text);
    void setRowDimmed(int row, bool dimmed);

    /// Runs the modal editor until the user cancels or the result is valid.
    /// replaceIndex is the row being edited, or -1 when adding a motor.
    [[nodiscard]] bool runConfigDialog(MotorConfig &config, int replaceIndex);
    [[nodiscard]] int findFeedbackIdConflict(quint32 feedbackId, int exceptIndex) const;
    [[nodiscard]] quint32 nextFreeDjiFeedbackId() const;
    [[nodiscard]] int selectedRow() const;

    [[nodiscard]] QString modelDisplayName(MotorModel model) const;
    [[nodiscard]] static QString formatCanId(quint32 canId);
};
