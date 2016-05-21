#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QWebEngineView>
#include <QtWebEngine/QtWebEngine>
#include <QDebug>

#include "ipcserver.hpp"
#include "eventmanager.hpp"
#include "commandhandler.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName("instant-webview");
    app.setApplicationVersion("1.0");

    QCommandLineParser cmdParser;
    cmdParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    cmdParser.setApplicationDescription("Instant WebView is a web browser which has no UI controls.");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(QCommandLineOption(QStringList() << "t" << "transport", "IPC Transport Layer to use.", "tcp|unixsocket"));
    cmdParser.addOption(QCommandLineOption(QStringList() << "r" << "reverse", "Enable reverse mode. The ID is used to identify your session in the server.", "ID"));
    cmdParser.process(app);

    if (cmdParser.value("transport").isEmpty()) {
        qDebug().noquote() << "You must provide a IPC transport layer";
        return -1;
    }

    QWebEngineView *webView = new QWebEngineView();
    webView->load(QUrl("about:blank"));

    EventManager *eventManager = new EventManager;
    eventManager->setWebView(webView);
    eventManager->bind();

    IpcServer *ipcServer = new IpcServer();
    ipcServer->setTransport(cmdParser.value("transport"));
    ipcServer->setReverse(cmdParser.isSet("reverse"));
    ipcServer->setReverseId(cmdParser.value("reverse"));
    ipcServer->initialize();

    CommandHandler *commandHandler = new CommandHandler();
    commandHandler->setEventManager(eventManager);

    QObject::connect(ipcServer, &IpcServer::newCommand, commandHandler, &CommandHandler::processCommand);

    return app.exec();
}
