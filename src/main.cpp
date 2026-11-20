/*
  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
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
#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFileInfo>
#include <QTranslator>

#include <core/Log.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("CANgaroo/Schildkroet");
    QCoreApplication::setOrganizationDomain("CANgaroo/Schildkroet");
    QCoreApplication::setApplicationName("CANgaroo");

    QCoreApplication::setApplicationVersion(QStringLiteral(VERSION_STRING));

    QCommandLineParser parser;
    parser.setApplicationDescription("CANgaroo CAN Bus Analyzer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("workspace", "Workspace file to load.", "[workspace]");

    const QCommandLineOption workspaceOption(
        {"w", "workspace"}, "Load workspace from <file>.", "file");
    parser.addOption(workspaceOption);
    parser.process(a);

    MainWindow w;

    // -w takes precedence over a positional argument (e.g. "Open with")
    QString workspaceFile = parser.value(workspaceOption);
    if (workspaceFile.isEmpty() && !parser.positionalArguments().isEmpty())
    {
        workspaceFile = parser.positionalArguments().constFirst();
    }

    if (!workspaceFile.isEmpty())
    {
        const QFileInfo fileInfo(workspaceFile);
        if (fileInfo.isFile())
        {
            w.loadWorkspaceFromFile(fileInfo.absoluteFilePath());
        }
        else
        {
            log_error(QObject::tr("Workspace file not found: %1").arg(workspaceFile));
        }
    }

    if(w.isMaximizedWindow())
    {
        w.showMaximized();
    }
    else
    {
        w.show();
    }
    return a.exec();
}
