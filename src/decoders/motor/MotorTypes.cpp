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

#include "MotorTypes.h"

namespace MotorProtocols {

bool isDjiModel(MotorModel model)
{
    return model == MotorModel::DjiM3508
        || model == MotorModel::DjiM2006
        || model == MotorModel::DjiGM6020;
}

bool isDamiaoModel(MotorModel model)
{
    return model == MotorModel::DamiaoMit;
}

QString modelKey(MotorModel model)
{
    switch (model) {
    case MotorModel::DjiM3508:  return QStringLiteral("dji_m3508");
    case MotorModel::DjiM2006:  return QStringLiteral("dji_m2006");
    case MotorModel::DjiGM6020: return QStringLiteral("dji_gm6020");
    case MotorModel::DamiaoMit: return QStringLiteral("damiao_mit");
    }
    return QStringLiteral("dji_m3508");
}

MotorModel modelFromKey(const QString &key, bool *ok)
{
    if (ok) {
        *ok = true;
    }
    if (key == QLatin1String("dji_m3508")) {
        return MotorModel::DjiM3508;
    }
    if (key == QLatin1String("dji_m2006")) {
        return MotorModel::DjiM2006;
    }
    if (key == QLatin1String("dji_gm6020")) {
        return MotorModel::DjiGM6020;
    }
    if (key == QLatin1String("damiao_mit")) {
        return MotorModel::DamiaoMit;
    }
    if (ok) {
        *ok = false;
    }
    return MotorModel::DjiM3508;
}

double djiNominalCurrentA(MotorModel model)
{
    switch (model) {
    case MotorModel::DjiM3508:  return 20.0;
    case MotorModel::DjiM2006:  return 0.0; // C610: raw torque, no A conversion documented
    case MotorModel::DjiGM6020: return 3.0;
    case MotorModel::DamiaoMit: return 0.0;
    }
    return 0.0;
}

bool djiCurrentConversionValid(MotorModel model)
{
    return model == MotorModel::DjiM3508 || model == MotorModel::DjiGM6020;
}

double djiCurrentPerRaw(MotorModel model)
{
    constexpr double rawScale = 16384.0;
    return djiNominalCurrentA(model) / rawScale;
}

} // namespace MotorProtocols
