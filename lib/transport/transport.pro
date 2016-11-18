TEMPLATE = lib
CONFIG += staticlib

QT += network websockets

INCLUDEPATH += \
    $$PWD/../

SOURCES += \
    $$PWD/../command/commandtransportlayer.cpp

HEADERS += \
    $$PWD/../command/commandtransportlayer.hpp

# TCP transport

SOURCES += \
    $$PWD/tcp/tcpcommandtransport.cpp

HEADERS += \
    $$PWD/tcp/tcpcommandtransport.hpp

# Unix Socket transport

SOURCES += \
    $$PWD/unixsocket/unixsocketcommandtransport.cpp

HEADERS += \
    $$PWD/unixsocket/unixsocketcommandtransport.hpp

# WebSocket

SOURCES += \
    $$PWD/websocket/websocketcommandtransport.cpp

HEADERS += \
    $$PWD/websocket/websocketcommandtransport.hpp
