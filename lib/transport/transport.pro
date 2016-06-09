TEMPLATE = lib
CONFIG += staticlib

QT += network websockets

INCLUDEPATH += \
    $$PWD/../

SOURCES += \
    $$PWD/../ipc/ipctransportlayer.cpp

HEADERS += \
    $$PWD/../ipc/ipctransportlayer.hpp

# TCP transport

SOURCES += \
    $$PWD/tcp/tcpipctransport.cpp

HEADERS += \
    $$PWD/tcp/tcpipctransport.hpp

# Unix Socket transport

SOURCES += \
    $$PWD/unixsocket/unixsocketipctransport.cpp

HEADERS += \
    $$PWD/unixsocket/unixsocketipctransport.hpp

# WebSocket

SOURCES += \
    $$PWD/websocket/websocketipctransport.cpp

HEADERS += \
    $$PWD/websocket/websocketipctransport.hpp
