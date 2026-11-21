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

#include "MotorDbc.h"

#include <cmath>

#include <QByteArray>
#include <QSet>
#include <QStringList>
#include <QTemporaryFile>

#include "db/dbc/DbcParser.h"

namespace {

constexpr int k_feedbackDlc = 8;
constexpr quint32 k_maxStandardId = 0x7FFu;

struct SignalDefinition
{
    QString name;
    int startBit = 0;
    int length = 0;
    bool isUnsigned = true;
    double factor = 1.0;
    double offset = 0.0;
    double minimum = 0.0;
    double maximum = 0.0;
    QString unit;
};

void setError(QString *error, const QString &message)
{
    if (error) {
        *error = message;
    }
}

/// DBC numbers must not use scientific notation: the parser's number token only
/// accepts decimal digits. Seventeen fractional digits are enough to round-trip
/// a double, which keeps the generated factors bit-for-bit close to the values
/// MotorDecoder computes (twelve digits, as used by many DBC writers, are not:
/// e.g. (2 * 12.5) / 65535 loses enough precision to move the physical result
/// by a few 1e-8 rad). Trailing zeroes are removed again for readability.
QString formatNumber(double value)
{
    if (value == 0.0) {
        return QStringLiteral("0");
    }
    QString text = QString::number(value, 'f', 17);
    while (text.endsWith(QLatin1Char('0'))) {
        text.chop(1);
    }
    if (text.endsWith(QLatin1Char('.'))) {
        text.chop(1);
    }
    if (text == QLatin1String("-0")) {
        text = QStringLiteral("0");
    }
    return text;
}

/// DBC identifiers may only contain ASCII letters, digits and underscores, and
/// must not start with a digit. Non-ASCII characters (e.g. the Chinese names
/// used by the motor view) are dropped; names that become empty fall back to
/// Motor<index>.
QString sanitizeIdentifier(const QString &name, int motorIndex)
{
    QString result;
    result.reserve(name.size());
    for (const QChar ch : name) {
        const ushort code = ch.unicode();
        const bool asciiLetter = (code >= 'A' && code <= 'Z') || (code >= 'a' && code <= 'z');
        const bool asciiDigit = code >= '0' && code <= '9';
        if (asciiLetter || asciiDigit || ch == QLatin1Char('_')) {
            result.append(ch);
        }
    }
    if (result.isEmpty()) {
        result = QStringLiteral("Motor%1").arg(motorIndex + 1);
    }
    if (result.at(0).isDigit()) {
        result.prepend(QLatin1Char('M'));
    }
    return result;
}

/// Appends _2, _3, ... until the identifier is unique within the database.
/// Exact duplicate configuration names are rejected by buildText(); this only
/// resolves collisions created by dropping unsupported characters.
QString uniqueIdentifier(const QString &base, QSet<QString> &used)
{
    QString candidate = base;
    int suffix = 2;
    while (used.contains(candidate)) {
        candidate = base + QLatin1Char('_') + QString::number(suffix++);
    }
    used.insert(candidate);
    return candidate;
}

QString formatSignal(const SignalDefinition &signal)
{
    return QStringLiteral(" SG_ %1 : %2|%3@0%4 (%5,%6) [%7|%8] \"%9\" Vector__XXX")
        .arg(signal.name)
        .arg(signal.startBit)
        .arg(signal.length)
        .arg(signal.isUnsigned ? QStringLiteral("+") : QStringLiteral("-"))
        .arg(formatNumber(signal.factor))
        .arg(formatNumber(signal.offset))
        .arg(formatNumber(signal.minimum))
        .arg(formatNumber(signal.maximum))
        .arg(signal.unit);
}

void appendDjiSignals(QStringList &lines, MotorModel model)
{
    SignalDefinition angle;
    angle.name = QStringLiteral("Angle");
    angle.startBit = 7;
    angle.length = 16;
    angle.factor = 360.0 / 8192.0;
    angle.minimum = 0.0;
    angle.maximum = 359.9560546875;
    angle.unit = QStringLiteral("deg");
    lines.append(formatSignal(angle));

    SignalDefinition speed;
    speed.name = QStringLiteral("Speed");
    speed.startBit = 23;
    speed.length = 16;
    speed.isUnsigned = false;
    speed.minimum = -32768.0;
    speed.maximum = 32767.0;
    speed.unit = QStringLiteral("rpm");
    lines.append(formatSignal(speed));

    if (model == MotorModel::DjiM2006) {
        // The C610 has no documented amps conversion for the feedback field;
        // keep it as the raw torque value instead of inventing a scale.
        SignalDefinition torqueRaw;
        torqueRaw.name = QStringLiteral("TorqueRaw");
        torqueRaw.startBit = 39;
        torqueRaw.length = 16;
        torqueRaw.isUnsigned = false;
        torqueRaw.minimum = -32768.0;
        torqueRaw.maximum = 32767.0;
        lines.append(formatSignal(torqueRaw));
        return;
    }

    const double nominalCurrent = MotorProtocols::djiNominalCurrentA(model);
    SignalDefinition current;
    current.name = QStringLiteral("Current");
    current.startBit = 39;
    current.length = 16;
    current.isUnsigned = false;
    current.factor = nominalCurrent / 16384.0;
    current.minimum = -nominalCurrent;
    current.maximum = nominalCurrent;
    current.unit = QStringLiteral("A");
    lines.append(formatSignal(current));

    SignalDefinition temperature;
    temperature.name = QStringLiteral("Temperature");
    temperature.startBit = 55;
    temperature.length = 8;
    temperature.minimum = 0.0;
    temperature.maximum = 255.0;
    temperature.unit = QStringLiteral("degC");
    lines.append(formatSignal(temperature));
}

void appendDamiaoSignals(QStringList &lines, const MotorConfig &config)
{
    const double pmax = config.damiaoPmax;
    const double vmax = config.damiaoVmax;
    const double tmax = config.damiaoTmax;

    SignalDefinition state;
    state.name = QStringLiteral("State");
    state.startBit = 7;
    state.length = 4;
    state.maximum = 15.0;
    lines.append(formatSignal(state));

    SignalDefinition position;
    position.name = QStringLiteral("Position");
    position.startBit = 15;
    position.length = 16;
    position.factor = (2.0 * pmax) / 65535.0;
    position.offset = -pmax;
    position.minimum = -pmax;
    position.maximum = pmax;
    position.unit = QStringLiteral("rad");
    lines.append(formatSignal(position));

    SignalDefinition velocity;
    velocity.name = QStringLiteral("Velocity");
    velocity.startBit = 31;
    velocity.length = 12;
    velocity.factor = (2.0 * vmax) / 4095.0;
    velocity.offset = -vmax;
    velocity.minimum = -vmax;
    velocity.maximum = vmax;
    velocity.unit = QStringLiteral("rad/s");
    lines.append(formatSignal(velocity));

    SignalDefinition torque;
    torque.name = QStringLiteral("Torque");
    torque.startBit = 35;
    torque.length = 12;
    torque.factor = (2.0 * tmax) / 4095.0;
    torque.offset = -tmax;
    torque.minimum = -tmax;
    torque.maximum = tmax;
    torque.unit = QStringLiteral("N\u00B7m");
    lines.append(formatSignal(torque));

    SignalDefinition mosTemperature;
    mosTemperature.name = QStringLiteral("MosTemperature");
    mosTemperature.startBit = 55;
    mosTemperature.length = 8;
    mosTemperature.minimum = 0.0;
    mosTemperature.maximum = 255.0;
    mosTemperature.unit = QStringLiteral("degC");
    lines.append(formatSignal(mosTemperature));

    SignalDefinition rotorTemperature;
    rotorTemperature.name = QStringLiteral("RotorTemperature");
    rotorTemperature.startBit = 63;
    rotorTemperature.length = 8;
    rotorTemperature.minimum = 0.0;
    rotorTemperature.maximum = 255.0;
    rotorTemperature.unit = QStringLiteral("degC");
    lines.append(formatSignal(rotorTemperature));
}

QString modelComment(MotorModel model)
{
    switch (model) {
    case MotorModel::DjiM3508:  return QStringLiteral("DJI M3508 feedback");
    case MotorModel::DjiM2006:  return QStringLiteral("DJI M2006 feedback");
    case MotorModel::DjiGM6020: return QStringLiteral("DJI GM6020 feedback");
    case MotorModel::DamiaoMit: return QStringLiteral("DAMIAO MIT feedback");
    }
    return QStringLiteral("Motor feedback");
}

QString stateValueTable(quint32 canId)
{
    return QStringLiteral("VAL_ %1 State 0 \"Disabled\" 1 \"Enabled\" 8 \"Overvoltage\" "
                          "9 \"Undervoltage\" 10 \"Overcurrent\" 11 \"MOS Overheat\" "
                          "12 \"Rotor Overheat\" 13 \"Communication Loss\" 14 \"Overload\" ;")
        .arg(QString::number(canId));
}

/// Moves the generated text into a temporary file and parses it with the same
/// DbcParser used for user supplied databases.
bool parseGeneratedText(const QString &text, pCanDb &database, QString *error)
{
    QTemporaryFile file;
    if (!file.open()) {
        setError(error, QStringLiteral("Cannot create temporary DBC file: %1")
                            .arg(file.errorString()));
        return false;
    }

    // DbcParser reads DBC files as Latin-1. Writing UTF-8 would turn the only
    // non-ASCII character in the generated database, the middle dot in "N·m",
    // into two Latin-1 characters ("NÂ·m"). The generated text is therefore
    // encoded as Latin-1; every character in it is representable in that
    // encoding.
    const QByteArray bytes = text.toLatin1();
    if (file.write(bytes) != bytes.size()) {
        setError(error, QStringLiteral("Cannot write temporary DBC file: %1")
                            .arg(file.errorString()));
        return false;
    }
    if (!file.flush() || !file.seek(0)) {
        setError(error, QStringLiteral("Cannot rewind temporary DBC file: %1")
                            .arg(file.errorString()));
        return false;
    }

    pCanDb parsed(new CanDb());
    DbcParser parser;
    if (!parser.parseFile(&file, *parsed)) {
        setError(error, QStringLiteral("Generated DBC failed to parse"));
        return false;
    }

    database = parsed;
    return true;
}

} // namespace

namespace MotorDbc {

QString buildText(const QVector<MotorConfig> &motors, QString *error)
{
    setError(error, QString());

    QVector<MotorConfig> activeMotors;
    activeMotors.reserve(motors.size());
    for (const MotorConfig &motor : motors) {
        if (motor.enabled) {
            activeMotors.append(motor);
        }
    }
    if (activeMotors.isEmpty()) {
        return QString();
    }

    QSet<quint32> feedbackIds;
    QSet<QString> motorNames;
    for (const MotorConfig &motor : activeMotors) {
        if (motor.feedbackId > k_maxStandardId) {
            setError(error, QStringLiteral("Feedback CAN ID 0x%1 does not fit into 11 bits")
                                .arg(QString::number(motor.feedbackId, 16)));
            return QString();
        }
        if (feedbackIds.contains(motor.feedbackId)) {
            setError(error, QStringLiteral("Duplicate feedback CAN ID 0x%1")
                                .arg(QString::number(motor.feedbackId, 16)));
            return QString();
        }
        feedbackIds.insert(motor.feedbackId);

        if (!motor.name.isEmpty()) {
            if (motorNames.contains(motor.name)) {
                setError(error, QStringLiteral("Duplicate motor name \"%1\"").arg(motor.name));
                return QString();
            }
            motorNames.insert(motor.name);
        }

        if (MotorProtocols::isDamiaoModel(motor.model)
            && (!std::isfinite(motor.damiaoPmax) || !std::isfinite(motor.damiaoVmax)
                || !std::isfinite(motor.damiaoTmax))) {
            setError(error, QStringLiteral("DAMIAO P/V/T ranges must be finite numbers"));
            return QString();
        }
    }

    QStringList lines;
    lines << QStringLiteral("VERSION \"\"") << QString() << QStringLiteral("NS_ :") << QString()
          << QStringLiteral("BS_:") << QString();

    QStringList nodeNames;
    QStringList messageNames;
    nodeNames.reserve(activeMotors.size());
    messageNames.reserve(activeMotors.size());

    QSet<QString> usedIdentifiers;
    for (int index = 0; index < activeMotors.size(); ++index) {
        nodeNames.append(QStringLiteral("Node%1").arg(index + 1));
        messageNames.append(uniqueIdentifier(
            sanitizeIdentifier(activeMotors.at(index).name, index), usedIdentifiers));
    }

    lines << QStringLiteral("BU_: %1").arg(nodeNames.join(QLatin1Char(' '))) << QString();

    for (int index = 0; index < activeMotors.size(); ++index) {
        const MotorConfig &motor = activeMotors.at(index);
        const QString canId = QString::number(motor.feedbackId);

        lines << QStringLiteral("BO_ %1 %2: %3 %4")
                     .arg(canId, messageNames.at(index))
                     .arg(k_feedbackDlc)
                     .arg(nodeNames.at(index));

        if (MotorProtocols::isDamiaoModel(motor.model)) {
            appendDamiaoSignals(lines, motor);
        } else {
            appendDjiSignals(lines, motor.model);
        }

        // The empty line terminates the BO_ section. Without it DbcParser would
        // try to read the following CM_ line as another SG_ of this message.
        lines << QString();

        lines << QStringLiteral("CM_ BO_ %1 \"%2\";")
                     .arg(canId, modelComment(motor.model));

        if (MotorProtocols::isDamiaoModel(motor.model)) {
            lines << stateValueTable(motor.feedbackId);
        }

        lines << QString();
    }

    return lines.join(QLatin1Char('\n'));
}

pCanDb buildDatabase(const QVector<MotorConfig> &motors, QString *error)
{
    setError(error, QString());

    QString buildError;
    const QString text = buildText(motors, &buildError);
    if (!buildError.isEmpty()) {
        setError(error, buildError);
        return pCanDb();
    }
    if (text.isEmpty()) {
        return pCanDb();
    }

    pCanDb database;
    if (!parseGeneratedText(text, database, error)) {
        return pCanDb();
    }
    return database;
}

} // namespace MotorDbc
