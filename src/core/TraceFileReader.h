/*
  Copyright (c) 2026 Schildkroet
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

#include <functional>
#include <optional>

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QVector>

#include "core/BusMessage.h"
#include "core/TraceFileFormat.h"

class QIODevice;

struct TraceReadResult
{
    QVector<BusMessage> messages;
    QVector<QString> interfaceNames;
    QDateTime headerStartTime;
    QStringList warnings;
    QString errorMessage;
    bool ok = false;
    bool cancelled = false;
};

namespace TraceFileReader
{

// Progress callback: returns false to cancel loading.
using ProgressFn = std::function<bool(qint64 bytesRead, qint64 totalBytes)>;

[[nodiscard]] TraceReadResult read(QIODevice &in, TraceFileFormat format,
                                   qint64 totalBytes = -1,
                                   const ProgressFn &progress = {});

[[nodiscard]] TraceReadResult readFile(const QString &filePath,
                                       std::optional<TraceFileFormat> explicitFormat = std::nullopt,
                                       const ProgressFn &progress = {});

// Splits an in-memory TraceReadResult into numbered files of at most
// `maxFramesPerPart` frames each using TraceFileWriter.
[[nodiscard]] bool writeSplitFiles(const TraceReadResult &data,
                                   const QString &outputDir,
                                   const QString &baseName,
                                   TraceFileFormat format,
                                   int maxFramesPerPart,
                                   QStringList *writtenFiles = nullptr,
                                   QString *errorMessage = nullptr);

} // namespace TraceFileReader
