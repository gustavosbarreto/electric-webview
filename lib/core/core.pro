TEMPLATE = lib
CONFIG += staticlib

QT += webenginewidgets

INCLUDEPATH += $$PWD/../

SOURCES += \
    $$PWD/eventmanager.cpp \
    $$PWD/commandhandler.cpp \
    $$PWD/command.cpp \
    $$PWD/event.cpp \
    $$PWD/inputeventfilter.cpp \
    $$PWD/electricwebview.cpp

HEADERS += \
    $$PWD/eventmanager.hpp \
    $$PWD/commandhandler.hpp \
    $$PWD/command.hpp \
    $$PWD/event.hpp \
    $$PWD/inputeventfilter.hpp \
    $$PWD/electricwebview.hpp
