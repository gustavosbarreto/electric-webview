TEMPLATE = subdirs

QMAKE_STRIP =

scripts.path = $$PREFIX/share/electric-webview/
scripts.files = $$PWD/httpserver.py

INSTALLS += scripts
