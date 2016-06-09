QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = instant-webview-ctl

INCLUDEPATH += \
    $$PWD/../../lib

SOURCES += \
    main.cpp

LIBS += \
    -L$$PWD/../../lib/core -lcore \
    -L$$PWD/../../lib/ipc -lipc \
    -L$$PWD/../../lib/transport -ltransport

scripts.files = $$PWD/shellscript.sh
scripts.path = $$PREFIX/share/instant-webview/

target.path = $$PREFIX/bin

INSTALLS += target scripts
