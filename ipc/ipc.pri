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
