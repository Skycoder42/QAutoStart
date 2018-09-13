INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
	$$PWD/qautostart.h

HEADERS += $$PUBLIC_HEADERS \
	$$PWD/qautostart_p.h

SOURCES += \
	$$PWD/qautostart.cpp

win32: SOURCES += $$PWD/qautostart_win.cpp
else:mac: SOURCES += $$PWD/qautostart_mac.cpp
else:unix: SOURCES += $$PWD/qautostart_x11.cpp
