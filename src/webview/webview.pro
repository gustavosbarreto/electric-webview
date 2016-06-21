QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = instant-webview

INCLUDEPATH += \
    $$PWD/../../lib

SOURCES += \
    main.cpp \
    webpage.cpp

LIBS += \
    -L$$OUT_PWD/../../lib/core -lcore \
    -L$$OUT_PWD/../../lib/ipc -lipc \
    -L$$OUT_PWD/../../lib/transport -ltransport

RESOURCES += resources.qrc

target.path = $$PREFIX/bin

INSTALLS += target

HEADERS += \
    webpage.hpp
