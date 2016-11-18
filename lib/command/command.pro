TEMPLATE = lib
CONFIG += staticlib

QT += network

INCLUDEPATH += \
    $$PWD/../

LIBS += \
    -L$$OUT_PWD/../core -lcore \
    -L$$OUT_PWD/../transport -ltransport

# Server

SOURCES += \
    $$PWD/commandserver.cpp

HEADERS += \
    $$PWD/commandserver.hpp

# Abstract classes

SOURCES += \
    $$PWD/commandclient.cpp \
    $$PWD/commandtransportlayer.cpp

HEADERS += \
    $$PWD/commandclient.hpp \
    $$PWD/commandtransportlayer.hpp
