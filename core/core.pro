QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = instant-webview

INCLUDEPATH += \
    $$PWD/../ipc \
    $$PWD/../transport \
    $$PWD/../transport/tcp \
    $$PWD/../transport/unixsocket \
    $$PWD/../transport/websocket

include($$PWD/../transport/transport.pri)
include($$PWD/../ipc/ipc.pri)

SOURCES += \
    main.cpp \
    eventmanager.cpp \
    commandhandler.cpp \
    command.cpp \
    event.cpp

HEADERS += \
    eventmanager.hpp \
    commandhandler.hpp \
    command.hpp \
    event.hpp

target.path = $$PREFIX/bin

INSTALLS += target
