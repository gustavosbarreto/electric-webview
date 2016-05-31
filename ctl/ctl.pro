QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = instant-webview-ctl

INCLUDEPATH += \
    $$PWD/../core \
    $$PWD/../ipc \
    $$PWD/../transport \
    $$PWD/../transport/tcp \
    $$PWD/../transport/unixsocket \
    $$PWD/../transport/websocket

include($$PWD/../transport/transport.pri)
include($$PWD/../ipc/ipc.pri)

SOURCES += \
    main.cpp \
    $$PWD/../core/command.cpp

HEADERS += \
    $$PWD/../core/command.hpp

scripts.files = $$PWD/shellscript.sh
scripts.path = $$PREFIX/share/instant-webview/

target.path = $$PREFIX/bin

INSTALLS += target scripts
