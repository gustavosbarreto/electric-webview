INCLUDEPATH += \
    $$PWD/../ipc \
    $$PWD/../transport \
    $$PWD/../transport/tcp \
    $$PWD/../transport/unixsocket \
    $$PWD/../transport/websocket

include($$PWD/../transport/transport.pri)
include($$PWD/../ipc/ipc.pri)

SOURCES += \
    $$PWD/eventmanager.cpp \
    $$PWD/commandhandler.cpp \
    $$PWD/command.cpp \
    $$PWD/event.cpp \
    $$PWD/inputeventfilter.cpp \
    $$PWD/instantwebview.cpp

HEADERS += \
    $$PWD/eventmanager.hpp \
    $$PWD/commandhandler.hpp \
    $$PWD/command.hpp \
    $$PWD/event.hpp \
    $$PWD/inputeventfilter.hpp \
    $$PWD/instantwebview.hpp
