TEMPLATE = app

QT += core gui widgets

TARGET = Sample

include(../qautostart.pri)

HEADERS += \
		testwidget.h

SOURCES += \
		main.cpp \
		testwidget.cpp

FORMS += \
		testwidget.ui
