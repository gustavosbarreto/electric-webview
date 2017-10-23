QT = gui widgets webenginewidgets websockets network

CONFIG += c++11

TARGET = electric-webview

INCLUDEPATH += \
    $$PWD/../../lib

SOURCES += \
    main.cpp \
    webpage.cpp

LIBS += \
    -L$$OUT_PWD/../../lib/core -lcore \
    -L$$OUT_PWD/../../lib/command -lcommand \
    -L$$OUT_PWD/../../lib/transport -ltransport

RESOURCES += resources.qrc

target.path = $$PREFIX/bin

INSTALLS += target

HEADERS += \
    webpage.hpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/command/release/ -lcommand
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/command/debug/ -lcommand
else:unix: LIBS += -L$$OUT_PWD/../../lib/command/ -lcommand

INCLUDEPATH += $$PWD/../../lib/command
DEPENDPATH += $$PWD/../../lib/command

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/command/release/libcommand.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/command/debug/libcommand.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/command/release/command.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/command/debug/command.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../lib/command/libcommand.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../../lib/core/ -lcore

INCLUDEPATH += $$PWD/../../lib/core
DEPENDPATH += $$PWD/../../lib/core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../lib/core/libcore.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/transport/release/ -ltransport
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/transport/debug/ -ltransport
else:unix: LIBS += -L$$OUT_PWD/../../lib/transport/ -ltransport

INCLUDEPATH += $$PWD/../../lib/transport
DEPENDPATH += $$PWD/../../lib/transport

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/transport/release/libtransport.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/transport/debug/libtransport.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/transport/release/transport.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/transport/debug/transport.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../lib/transport/libtransport.a
