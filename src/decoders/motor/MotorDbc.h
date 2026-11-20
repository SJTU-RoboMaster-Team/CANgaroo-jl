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
#include <QVector>

#include "db/model/CanDb.h"
#include "decoders/motor/MotorTypes.h"

/// Generates a CAN database from the configured motors.
///
/// The generated text is a regular DBC file: it is parsed back with DbcParser
/// so Trace/Graph/trigger rules can treat the motor feedback frames exactly like
/// any user supplied database. Because DAMIAO motor IDs and P/V/T ranges are
/// configuration, the database is generated per configuration instead of being
/// shipped as a static file.
namespace MotorDbc {

/// Returns the generated DBC text. Disabled motors are skipped; an empty (or
/// all-disabled) configuration yields an empty string and no error. Invalid
/// configurations (duplicate CAN IDs, IDs that do not fit into 11 bits,
/// duplicate motor names) yield an empty string and an English reason in
/// `*error`.
[[nodiscard]] QString buildText(const QVector<MotorConfig> &motors, QString *error = nullptr);

/// buildText() -> temporary file -> DbcParser -> CanDb. Returns a null pointer
/// when the configuration is empty, all motors are disabled or invalid (see
/// `*error`). CanDb::getPath() names the temporary file the database was parsed
/// from; that file is removed when the call returns.
[[nodiscard]] pCanDb buildDatabase(const QVector<MotorConfig> &motors, QString *error = nullptr);

} // namespace MotorDbc
