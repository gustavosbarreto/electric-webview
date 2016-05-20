QT = gui widgets webenginewidgets network

CONFIG += c++11

TARGET = instant-webview

INCLUDEPATH += \
    $$PWD/../ipc \
    $$PWD/../transport \
    $$PWD/../transport/tcp \
    $$PWD/../transport/unixsocket

include($$PWD/../transport/transport.pri)
include($$PWD/../ipc/ipc.pri)

SOURCES += \
    main.cpp \
    eventmanager.cpp \
    commandhandler.cpp

HEADERS += \
    eventmanager.hpp \
    commandhandler.hpp
