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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a
