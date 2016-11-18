#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QWebEngineView>
#include <QDebug>

#include "webpage.hpp"

#include <command/commandserver.hpp>
#include <core/commandhandler.hpp>
#include <core/electricwebview.hpp>

#include <transport/unixsocket/unixsocketcommandtransport.hpp>
#include <transport/tcp/tcpcommandtransport.hpp>
#include <transport/websocket/websocketcommandtransport.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName("electric-webview");
    app.setApplicationVersion("1.0");

    QCommandLineParser cmdParser;
    cmdParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    cmdParser.setApplicationDescription("Electric WebView is a scriptable WebView for developers.");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(QCommandLineOption(QStringList() << "t" << "transport", "Command Transport Layer to use.", "tcp|unixsocket|websocket"));
    cmdParser.addOption(QCommandLineOption(QStringList() << "r" << "reverse", "Enable reverse mode. The ID is used to identify your session in the server.", "ID"));
    cmdParser.addOption(QCommandLineOption(QStringList() << "s" << "script", "Script to run.", "path"));
    cmdParser.process(app);

    if (cmdParser.value("transport").isEmpty()) {
        qDebug().noquote() << "You must provide a command transport layer";
        return -1;
    }

    CommandServer *commandServer = new CommandServer();
    commandServer->setTransport(cmdParser.value("transport"));
    commandServer->setReverse(cmdParser.isSet("reverse"));
    commandServer->setReverseId(cmdParser.value("reverse"));
    commandServer->initialize();

    QWebEngineView *webView = new QWebEngineView;
    webView->setPage(new WebPage);

    ElectricWebView::instance()->setWebView(webView);

    ElectricWebView::instance()->initialize();

    QObject::connect(commandServer, &CommandServer::newCommand, ElectricWebView::instance()->commandHandler(), &CommandHandler::processCommand);

    if (cmdParser.isSet("script"))
        ElectricWebView::instance()->runScript(cmdParser.value("transport"), cmdParser.value("script"));

    return app.exec();
}
