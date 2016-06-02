QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = instant-webview

include($$PWD/../core/core.pri)

INCLUDEPATH += \
    $$PWD/../core

SOURCES += \
    main.cpp

RESOURCES += resources.qrc

target.path = $$PREFIX/bin

INSTALLS += target
