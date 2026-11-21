lessThan(QT_MAJOR_VERSION, 6): error("requires Qt 6")

QT += core gui
QT += widgets
QT += xml
QT += charts
QT += svg
QT += network

TARGET = cangaroo
TEMPLATE = app
CONFIG += warn_on

VERSION = 0.7.2
DEFINES += VERSION_STRING=\\\"$${VERSION}\\\"

TRANSLATIONS = \
    translations/i18n_zh_cn.ts
RC_ICONS = cangaroo.ico

INCLUDEPATH += $$PWD

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac


SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES = cangaroo.qrc

include($$PWD/core/core.pri)
include($$PWD/driver/driver.pri)
include($$PWD/db/db.pri)
include($$PWD/decoders/decoders.pri)
include($$PWD/window/window.pri)
include($$PWD/helpers/helpers.pri)


win32:include($$PWD/driver/CandleApiDriver/CandleApiDriver.pri)

DISTFILES += \
    assets/filter-symbolic.svg
