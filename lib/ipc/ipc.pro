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
    $$PWD/ipcserver.cpp

HEADERS += \
    $$PWD/ipcserver.hpp

# Abstract classes

SOURCES += \
    $$PWD/ipcclient.cpp \
    $$PWD/ipctransportlayer.cpp

HEADERS += \
    $$PWD/ipcclient.hpp \
    $$PWD/ipctransportlayer.hpp
