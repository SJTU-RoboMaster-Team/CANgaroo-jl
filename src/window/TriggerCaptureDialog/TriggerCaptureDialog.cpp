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

#include "TriggerCaptureDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStandardPaths>
#include <QTableWidget>
#include <QVBoxLayout>

namespace
{
QString opText(TriggerRule::CompareOp op)
{
    switch (op) {
    case TriggerRule::CompareOp::Equal:        return QStringLiteral("==");
    case TriggerRule::CompareOp::NotEqual:     return QStringLiteral("!=");
    case TriggerRule::CompareOp::Greater:      return QStringLiteral(">");
    case TriggerRule::CompareOp::Less:         return QStringLiteral("<");
    case TriggerRule::CompareOp::GreaterEqual: return QStringLiteral(">=");
    case TriggerRule::CompareOp::LessEqual:    return QStringLiteral("<=");
    }
    return QStringLiteral("==");
}
} // namespace

TriggerCaptureDialog::TriggerCaptureDialog(const TriggerCaptureConfig &config,
                                           bool currentlyArmed,
                                           QWidget *parent)
    : QDialog(parent)
    , _rules(config.rules)
{
    setWindowTitle(tr("Trigger Capture (Blackbox)"));
    resize(700, 520);

    auto *mainLayout = new QVBoxLayout(this);

    auto *outGroup = new QGroupBox(tr("Output & Capture Window"), this);
    auto *outForm = new QFormLayout(outGroup);

    auto *folderRow = new QHBoxLayout();
    _folderEdit = new QLineEdit(config.outputFolder.isEmpty()
                                    ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                    : config.outputFolder,
                                this);
    auto *browseBtn = new QPushButton(tr("Browse..."), this);
    connect(browseBtn, &QPushButton::clicked, this, &TriggerCaptureDialog::browseFolder);
    folderRow->addWidget(_folderEdit, 1);
    folderRow->addWidget(browseBtn);
    outForm->addRow(tr("Output Folder:"), folderRow);

    auto *prefixFormatRow = new QHBoxLayout();
    _prefixEdit = new QLineEdit(config.filePrefix.isEmpty() ? QStringLiteral("trigger") : config.filePrefix, this);
    _formatCombo = new QComboBox(this);
    _formatCombo->addItem(tr("CAN/FD Raw CSV (*.csv)"), static_cast<int>(TraceFileFormat::Csv));
    _formatCombo->addItem(tr("Vector ASC (*.asc)"), static_cast<int>(TraceFileFormat::VectorAsc));
    _formatCombo->addItem(tr("SocketCAN candump (*.candump)"), static_cast<int>(TraceFileFormat::CanDump));
    _formatCombo->addItem(tr("PCAPng (*.pcapng)"), static_cast<int>(TraceFileFormat::PcapNg));
    _formatCombo->addItem(tr("Vector MDF4 (*.mf4)"), static_cast<int>(TraceFileFormat::VectorMdf));
    const int fmtIdx = _formatCombo->findData(static_cast<int>(config.format));
    if (fmtIdx >= 0) {
        _formatCombo->setCurrentIndex(fmtIdx);
    }
    prefixFormatRow->addWidget(_prefixEdit, 1);
    prefixFormatRow->addWidget(_formatCombo, 1);
    outForm->addRow(tr("Prefix / Format:"), prefixFormatRow);

    auto *windowRow = new QHBoxLayout();
    _preSecSpin = new QDoubleSpinBox(this);
    _preSecSpin->setRange(0.1, 60.0);
    _preSecSpin->setDecimals(1);
    _preSecSpin->setSuffix(QStringLiteral(" s"));
    _preSecSpin->setValue(config.preTriggerSeconds);

    _postSecSpin = new QDoubleSpinBox(this);
    _postSecSpin->setRange(0.1, 60.0);
    _postSecSpin->setDecimals(1);
    _postSecSpin->setSuffix(QStringLiteral(" s"));
    _postSecSpin->setValue(config.postTriggerSeconds);

    _maxPreFramesSpin = new QSpinBox(this);
    _maxPreFramesSpin->setRange(500, 200000);
    _maxPreFramesSpin->setSingleStep(5000);
    _maxPreFramesSpin->setValue(config.maxPreTriggerFrames);

    windowRow->addWidget(new QLabel(tr("Pre:"), this));
    windowRow->addWidget(_preSecSpin);
    windowRow->addWidget(new QLabel(tr("Post:"), this));
    windowRow->addWidget(_postSecSpin);
    windowRow->addWidget(new QLabel(tr("Max Pre-Frames:"), this));
    windowRow->addWidget(_maxPreFramesSpin);
    outForm->addRow(tr("Ring Buffer:"), windowRow);

    auto *optRow = new QHBoxLayout();
    _holdoffSpin = new QSpinBox(this);
    _holdoffSpin->setRange(0, 60000);
    _holdoffSpin->setSingleStep(500);
    _holdoffSpin->setSuffix(QStringLiteral(" ms"));
    _holdoffSpin->setValue(config.holdoffMs);

    _oneShotCheck = new QCheckBox(tr("One-shot (disarm after first capture)"), this);
    _oneShotCheck->setChecked(config.oneShot);

    _armCheck = new QCheckBox(tr("Arm trigger capture on OK"), this);
    _armCheck->setChecked(currentlyArmed || !_rules.isEmpty());

    optRow->addWidget(new QLabel(tr("Holdoff:"), this));
    optRow->addWidget(_holdoffSpin);
    optRow->addWidget(_oneShotCheck);
    optRow->addStretch();
    optRow->addWidget(_armCheck);
    outForm->addRow(tr("Behavior:"), optRow);

    mainLayout->addWidget(outGroup);

    auto *rulesGroup = new QGroupBox(tr("Trigger Rules (double-click a rule to edit)"), this);
    auto *rulesLayout = new QVBoxLayout(rulesGroup);

    _ruleTable = new QTableWidget(0, 4, this);
    _ruleTable->setHorizontalHeaderLabels({tr("On"), tr("Name"), tr("Type"), tr("Condition")});
    _ruleTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _ruleTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    _ruleTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    _ruleTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    _ruleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ruleTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _ruleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(_ruleTable, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        if (row >= 0 && row < _rules.size()) {
            if (editRuleDialog(_rules[row])) {
                updateRuleSummaryRow(row);
            }
        }
    });
    rulesLayout->addWidget(_ruleTable);

    auto *ruleBtnRow = new QHBoxLayout();
    auto *addBtn = new QPushButton(tr("Add Rule..."), this);
    auto *delBtn = new QPushButton(tr("Remove Rule"), this);
    connect(addBtn, &QPushButton::clicked, this, &TriggerCaptureDialog::addRule);
    connect(delBtn, &QPushButton::clicked, this, &TriggerCaptureDialog::removeSelectedRule);
    ruleBtnRow->addWidget(addBtn);
    ruleBtnRow->addWidget(delBtn);
    ruleBtnRow->addStretch();
    rulesLayout->addLayout(ruleBtnRow);

    mainLayout->addWidget(rulesGroup, 1);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (_armCheck->isChecked() && _folderEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Trigger Capture"), tr("Please select an output folder."));
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);

    populateRuleTable();
}

TriggerCaptureConfig TriggerCaptureDialog::config() const
{
    TriggerCaptureConfig cfg;
    cfg.outputFolder = _folderEdit->text().trimmed();
    cfg.filePrefix = _prefixEdit->text().trimmed();
    if (cfg.filePrefix.isEmpty()) {
        cfg.filePrefix = QStringLiteral("trigger");
    }
    cfg.format = static_cast<TraceFileFormat>(_formatCombo->currentData().toInt());
    cfg.preTriggerSeconds = _preSecSpin->value();
    cfg.postTriggerSeconds = _postSecSpin->value();
    cfg.maxPreTriggerFrames = _maxPreFramesSpin->value();
    cfg.holdoffMs = _holdoffSpin->value();
    cfg.oneShot = _oneShotCheck->isChecked();
    cfg.rules = _rules;
    for (int r = 0; r < _ruleTable->rowCount() && r < cfg.rules.size(); ++r) {
        if (auto *item = _ruleTable->item(r, 0)) {
            cfg.rules[r].enabled = (item->checkState() == Qt::Checked);
        }
    }
    return cfg;
}

bool TriggerCaptureDialog::shouldArm() const
{
    return _armCheck->isChecked();
}

TriggerRule TriggerCaptureDialog::makeQuickFrameRule(uint32_t canId)
{
    TriggerRule r;
    r.name = QStringLiteral("ID 0x%1").arg(canId, 0, 16).toUpper();
    r.kind = TriggerRule::Kind::FrameMatch;
    r.idMin = canId;
    r.idMax = canId;
    return r;
}

TriggerRule TriggerCaptureDialog::makeQuickWatchdogRule(uint32_t canId, int timeoutMs)
{
    TriggerRule r;
    r.name = QStringLiteral("Watchdog 0x%1").arg(canId, 0, 16).toUpper();
    r.kind = TriggerRule::Kind::Watchdog;
    r.idMin = canId;
    r.idMax = canId;
    r.watchdogTimeoutMs = timeoutMs;
    return r;
}

void TriggerCaptureDialog::browseFolder()
{
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select Trigger Output Folder"), _folderEdit->text());
    if (!dir.isEmpty()) {
        _folderEdit->setText(dir);
    }
}

void TriggerCaptureDialog::addRule()
{
    TriggerRule r;
    r.name = tr("Rule %1").arg(_rules.size() + 1);
    r.kind = TriggerRule::Kind::FrameMatch;
    if (editRuleDialog(r)) {
        _rules.append(r);
        appendRuleRow(r);
    }
}

void TriggerCaptureDialog::removeSelectedRule()
{
    const int row = _ruleTable->currentRow();
    if (row >= 0 && row < _rules.size()) {
        _rules.removeAt(row);
        _ruleTable->removeRow(row);
    }
}

void TriggerCaptureDialog::populateRuleTable()
{
    _ruleTable->setRowCount(0);
    for (const auto &r : _rules) {
        appendRuleRow(r);
    }
}

void TriggerCaptureDialog::appendRuleRow(const TriggerRule &rule)
{
    const int row = _ruleTable->rowCount();
    _ruleTable->insertRow(row);
    auto *enItem = new QTableWidgetItem();
    enItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    enItem->setCheckState(rule.enabled ? Qt::Checked : Qt::Unchecked);
    _ruleTable->setItem(row, 0, enItem);
    _ruleTable->setItem(row, 1, new QTableWidgetItem(rule.name));
    _ruleTable->setItem(row, 2, new QTableWidgetItem(ruleKindLabel(rule.kind)));
    _ruleTable->setItem(row, 3, new QTableWidgetItem(ruleDetailSummary(rule)));
}

void TriggerCaptureDialog::updateRuleSummaryRow(int row)
{
    if (row < 0 || row >= _rules.size() || row >= _ruleTable->rowCount()) {
        return;
    }
    const TriggerRule &r = _rules.at(row);
    if (auto *it = _ruleTable->item(row, 0)) it->setCheckState(r.enabled ? Qt::Checked : Qt::Unchecked);
    if (auto *it = _ruleTable->item(row, 1)) it->setText(r.name);
    if (auto *it = _ruleTable->item(row, 2)) it->setText(ruleKindLabel(r.kind));
    if (auto *it = _ruleTable->item(row, 3)) it->setText(ruleDetailSummary(r));
}

QString TriggerCaptureDialog::ruleKindLabel(TriggerRule::Kind kind) const
{
    switch (kind) {
    case TriggerRule::Kind::BusErrorEvent:   return tr("HW Bus Error");
    case TriggerRule::Kind::FrameMatch:      return tr("Frame Match");
    case TriggerRule::Kind::Watchdog:        return tr("Watchdog Timeout");
    case TriggerRule::Kind::SignalThreshold: return tr("DBC Signal");
    }
    return tr("Unknown");
}

QString TriggerCaptureDialog::ruleDetailSummary(const TriggerRule &r) const
{
    switch (r.kind) {
    case TriggerRule::Kind::BusErrorEvent:
        return r.errorMask == BusErrors{} ? tr("Any CAN/FD error frame") : tr("Error mask 0x%1").arg(static_cast<uint32_t>(r.errorMask), 0, 16);
    case TriggerRule::Kind::FrameMatch: {
        QString s = QStringLiteral("ID 0x%1..0x%2").arg(r.idMin, 0, 16).arg(r.idMax, 0, 16).toUpper();
        if (r.byteIndex >= 0) {
            s += QStringLiteral(" | (B[%1]&0x%2) %3 0x%4")
                     .arg(r.byteIndex)
                     .arg(r.byteMask, 2, 16, QLatin1Char('0'))
                     .arg(opText(r.byteOp))
                     .arg(r.byteValue, 2, 16, QLatin1Char('0'));
        }
        return s;
    }
    case TriggerRule::Kind::Watchdog:
        return tr("ID 0x%1..0x%2 silent > %3 ms")
            .arg(r.idMin, 0, 16)
            .arg(r.idMax, 0, 16)
            .arg(r.watchdogTimeoutMs);
    case TriggerRule::Kind::SignalThreshold:
        return QStringLiteral("%1.%2 %3 %4")
            .arg(r.signalMessageName.isEmpty() ? QStringLiteral("*") : r.signalMessageName,
                 r.signalName,
                 opText(r.signalOp))
            .arg(r.signalThreshold);
    }
    return {};
}

bool TriggerCaptureDialog::editRuleDialog(TriggerRule &rule)
{
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Edit Trigger Rule"));
    auto *form = new QFormLayout(&dlg);

    auto *nameEdit = new QLineEdit(rule.name, &dlg);
    auto *kindCombo = new QComboBox(&dlg);
    kindCombo->addItem(tr("HW Bus Error Frame"), static_cast<int>(TriggerRule::Kind::BusErrorEvent));
    kindCombo->addItem(tr("Software Frame / Mask Match"), static_cast<int>(TriggerRule::Kind::FrameMatch));
    kindCombo->addItem(tr("Watchdog Frame-Loss Timeout"), static_cast<int>(TriggerRule::Kind::Watchdog));
    kindCombo->addItem(tr("DBC Signal Threshold"), static_cast<int>(TriggerRule::Kind::SignalThreshold));
    kindCombo->setCurrentIndex(kindCombo->findData(static_cast<int>(rule.kind)));

    auto *idMinEdit = new QLineEdit(QString::number(rule.idMin, 16).toUpper(), &dlg);
    auto *idMaxEdit = new QLineEdit(QString::number(rule.idMax, 16).toUpper(), &dlg);
    auto *byteIdxSpin = new QSpinBox(&dlg);
    byteIdxSpin->setRange(-1, 63);
    byteIdxSpin->setSpecialValueText(tr("Disabled"));
    byteIdxSpin->setValue(rule.byteIndex);
    auto *byteMaskEdit = new QLineEdit(QString::number(rule.byteMask, 16).toUpper(), &dlg);
    auto *byteValEdit = new QLineEdit(QString::number(rule.byteValue, 16).toUpper(), &dlg);

    auto *opCombo = new QComboBox(&dlg);
    opCombo->addItem(QStringLiteral("=="), static_cast<int>(TriggerRule::CompareOp::Equal));
    opCombo->addItem(QStringLiteral("!="), static_cast<int>(TriggerRule::CompareOp::NotEqual));
    opCombo->addItem(QStringLiteral(">"), static_cast<int>(TriggerRule::CompareOp::Greater));
    opCombo->addItem(QStringLiteral("<"), static_cast<int>(TriggerRule::CompareOp::Less));
    opCombo->addItem(QStringLiteral(">="), static_cast<int>(TriggerRule::CompareOp::GreaterEqual));
    opCombo->addItem(QStringLiteral("<="), static_cast<int>(TriggerRule::CompareOp::LessEqual));
    opCombo->setCurrentIndex(opCombo->findData(static_cast<int>(
        rule.kind == TriggerRule::Kind::SignalThreshold ? rule.signalOp : rule.byteOp)));

    auto *watchdogSpin = new QSpinBox(&dlg);
    watchdogSpin->setRange(10, 60000);
    watchdogSpin->setSuffix(QStringLiteral(" ms"));
    watchdogSpin->setValue(rule.watchdogTimeoutMs);

    auto *sigMsgEdit = new QLineEdit(rule.signalMessageName, &dlg);
    sigMsgEdit->setPlaceholderText(tr("Optional DBC message name"));
    auto *sigNameEdit = new QLineEdit(rule.signalName, &dlg);
    sigNameEdit->setPlaceholderText(tr("DBC signal name (required)"));
    auto *sigThreshSpin = new QDoubleSpinBox(&dlg);
    sigThreshSpin->setRange(-1e9, 1e9);
    sigThreshSpin->setDecimals(4);
    sigThreshSpin->setValue(rule.signalThreshold);

    form->addRow(tr("Rule Name:"), nameEdit);
    form->addRow(tr("Trigger Kind:"), kindCombo);
    form->addRow(tr("ID Min (hex):"), idMinEdit);
    form->addRow(tr("ID Max (hex):"), idMaxEdit);
    form->addRow(tr("Byte Index (-1 off):"), byteIdxSpin);
    form->addRow(tr("Byte Mask (hex):"), byteMaskEdit);
    form->addRow(tr("Byte Value (hex):"), byteValEdit);
    form->addRow(tr("Compare Op:"), opCombo);
    form->addRow(tr("Watchdog Timeout:"), watchdogSpin);
    form->addRow(tr("DBC Message:"), sigMsgEdit);
    form->addRow(tr("DBC Signal:"), sigNameEdit);
    form->addRow(tr("Signal Threshold:"), sigThreshSpin);

    auto *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    form->addRow(btns);

    if (dlg.exec() != QDialog::Accepted) {
        return false;
    }

    rule.name = nameEdit->text().trimmed();
    rule.kind = static_cast<TriggerRule::Kind>(kindCombo->currentData().toInt());
    bool ok1 = false, ok2 = false;
    uint32_t idMin = idMinEdit->text().trimmed().toUInt(&ok1, 16);
    uint32_t idMax = idMaxEdit->text().trimmed().toUInt(&ok2, 16);
    if (ok1) rule.idMin = idMin;
    if (ok2) rule.idMax = idMax;
    if (rule.idMax < rule.idMin) rule.idMax = rule.idMin;

    rule.byteIndex = byteIdxSpin->value();
    bool okM = false, okV = false;
    uint32_t m = byteMaskEdit->text().trimmed().toUInt(&okM, 16);
    uint32_t v = byteValEdit->text().trimmed().toUInt(&okV, 16);
    if (okM) rule.byteMask = static_cast<uint8_t>(m & 0xFF);
    if (okV) rule.byteValue = static_cast<uint8_t>(v & 0xFF);

    const auto op = static_cast<TriggerRule::CompareOp>(opCombo->currentData().toInt());
    rule.byteOp = op;
    rule.signalOp = op;
    rule.watchdogTimeoutMs = watchdogSpin->value();
    rule.signalMessageName = sigMsgEdit->text().trimmed();
    rule.signalName = sigNameEdit->text().trimmed();
    rule.signalThreshold = sigThreshSpin->value();
    return true;
}
