QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = electric-webview

INCLUDEPATH += \
    $$PWD/../../lib

SOURCES += \
    main.cpp \
    webpage.cpp

LIBS += \
    -L$$OUT_PWD/../../lib/core -lcore \
    -L$$OUT_PWD/../../lib/command -lcommand \
    -L$$OUT_PWD/../../lib/transport -ltransport

RESOURCES += resources.qrc

target.path = $$PREFIX/bin

INSTALLS += target

HEADERS += \
    webpage.hpp
