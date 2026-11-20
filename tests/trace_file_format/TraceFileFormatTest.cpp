/*

  Copyright (c) 2026 Schildkroet

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

// Trace export format resolution.
//
// Both the GUI save dialog and the recorder route through this mapping, so
// picking the wrong format here means writing a file whose contents do not
// match its extension.

#include <optional>

#include <QtTest>

#include "core/TraceFileFormat.h"

class TraceFileFormatTest : public QObject
{
    Q_OBJECT

private slots:
    void fromPath_data();
    void fromPath();

    void fromPathIsCaseInsensitive();
    void fromPathRejectsUnknownExtension_data();
    void fromPathRejectsUnknownExtension();
    void fromPathHandlesDotsInDirectories();

    void fromName_data();
    void fromName();

    void fromNameAcceptsAliases();
    void fromNameIsCaseAndSpaceInsensitive();
    void fromNameRejectsUnknown();

    void everyFormatHasNameAndExtension();
    void canonicalNamesAreAllSupported();
    void extensionsRoundTripThroughFromPath();
};

void TraceFileFormatTest::fromPath_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("expected");

    QTest::newRow("candump") << "/tmp/run.candump" << int(TraceFileFormat::CanDump);
    QTest::newRow("log")     << "/tmp/run.log"     << int(TraceFileFormat::CanDump);
    QTest::newRow("asc")     << "/tmp/run.asc"     << int(TraceFileFormat::VectorAsc);
    QTest::newRow("csv")     << "/tmp/run.csv"     << int(TraceFileFormat::Csv);
    QTest::newRow("bare name") << "run.asc"        << int(TraceFileFormat::VectorAsc);
}

void TraceFileFormatTest::fromPath()
{
    QFETCH(QString, path);
    QFETCH(int, expected);

    const auto format = traceFormatFromPath(path);
    QVERIFY(format.has_value());
    QCOMPARE(int(*format), expected);
}

void TraceFileFormatTest::fromPathIsCaseInsensitive()
{
    for (const QString &path : { QStringLiteral("/tmp/RUN.ASC"),
                                 QStringLiteral("/tmp/run.Asc"),
                                 QStringLiteral("/tmp/run.aSC") })
    {
        const auto format = traceFormatFromPath(path);
        QVERIFY2(format.has_value(), qPrintable(path));
        QCOMPARE(*format, TraceFileFormat::VectorAsc);
    }

    QCOMPARE(traceFormatFromPath("/tmp/run.CSV"), TraceFileFormat::Csv);
}

void TraceFileFormatTest::fromPathRejectsUnknownExtension_data()
{
    QTest::addColumn<QString>("path");

    QTest::newRow("no extension")   << "/tmp/run";
    QTest::newRow("trailing dot")   << "/tmp/run.";
    QTest::newRow("unknown ext")    << "/tmp/run.txt";
    QTest::newRow("blf")            << "/tmp/run.blf";   // not implemented
    QTest::newRow("mf4")            << "/tmp/run.mf4";   // dropped with the MDF writer
    QTest::newRow("pcap")           << "/tmp/run.pcap";  // dropped with the pcap writer
    QTest::newRow("empty")          << "";
    QTest::newRow("just a dot")     << ".";
}

// Callers report an error rather than guessing; the GUI applies its own ASC
// default on top of a nullopt.
void TraceFileFormatTest::fromPathRejectsUnknownExtension()
{
    QFETCH(QString, path);
    QVERIFY(!traceFormatFromPath(path).has_value());
}

// The last dot decides, not the first -- a dotted directory must not be read as
// the extension.
void TraceFileFormatTest::fromPathHandlesDotsInDirectories()
{
    QCOMPARE(traceFormatFromPath("/home/u/my.traces/run.asc"), TraceFileFormat::VectorAsc);
    QVERIFY(!traceFormatFromPath("/home/u/my.traces/run").has_value());
}

void TraceFileFormatTest::fromName_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("expected");

    QTest::newRow("candump") << "candump" << int(TraceFileFormat::CanDump);
    QTest::newRow("asc")     << "asc"     << int(TraceFileFormat::VectorAsc);
    QTest::newRow("csv")     << "csv"     << int(TraceFileFormat::Csv);
}

void TraceFileFormatTest::fromName()
{
    QFETCH(QString, name);
    QFETCH(int, expected);

    const auto format = traceFormatFromName(name);
    QVERIFY(format.has_value());
    QCOMPARE(int(*format), expected);
}

void TraceFileFormatTest::fromNameAcceptsAliases()
{
    QCOMPARE(traceFormatFromName("log"), TraceFileFormat::CanDump);
    QCOMPARE(traceFormatFromName("vector_asc"), TraceFileFormat::VectorAsc);
}

void TraceFileFormatTest::fromNameIsCaseAndSpaceInsensitive()
{
    QCOMPARE(traceFormatFromName("ASC"), TraceFileFormat::VectorAsc);
    QCOMPARE(traceFormatFromName("Csv"), TraceFileFormat::Csv);
    QCOMPARE(traceFormatFromName("  asc  "), TraceFileFormat::VectorAsc);
    QCOMPARE(traceFormatFromName("\tcandump\n"), TraceFileFormat::CanDump);
}

void TraceFileFormatTest::fromNameRejectsUnknown()
{
    for (const char *name : { "", "  ", "blf", "txt", "vector", "asc2", "mf4", "pcapng" })
    {
        QVERIFY2(!traceFormatFromName(QString::fromLatin1(name)).has_value(), name);
    }
}

// Guards against a new enumerator being added without its name/extension case,
// which the switch statements would otherwise return an empty string for.
void TraceFileFormatTest::everyFormatHasNameAndExtension()
{
    for (const TraceFileFormat format : { TraceFileFormat::CanDump,
                                          TraceFileFormat::VectorAsc,
                                          TraceFileFormat::Csv })
    {
        QVERIFY(!traceFormatName(format).isEmpty());
        QVERIFY(!traceFormatExtension(format).isEmpty());
    }
}

// Every name the error messages advertise must actually resolve.
void TraceFileFormatTest::canonicalNamesAreAllSupported()
{
    const QStringList names = supportedTraceFormatNames();
    QCOMPARE(names.size(), 3);

    for (const QString &name : names)
    {
        const auto format = traceFormatFromName(name);
        QVERIFY2(format.has_value(), qPrintable(name));
        QCOMPARE(traceFormatName(*format), name);
    }
}

// A file named with a format's own extension must resolve back to that format.
void TraceFileFormatTest::extensionsRoundTripThroughFromPath()
{
    for (const TraceFileFormat format : { TraceFileFormat::CanDump,
                                          TraceFileFormat::VectorAsc,
                                          TraceFileFormat::Csv })
    {
        const QString path = "/tmp/trace." + traceFormatExtension(format);
        const auto resolved = traceFormatFromPath(path);
        QVERIFY2(resolved.has_value(), qPrintable(path));
        QCOMPARE(*resolved, format);
    }
}

QTEST_APPLESS_MAIN(TraceFileFormatTest)

#include "TraceFileFormatTest.moc"
