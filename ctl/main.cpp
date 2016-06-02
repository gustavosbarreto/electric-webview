#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <QSocketNotifier>
#include <QTextStream>
#include <QDebug>
#include <QFile>

#include <unistd.h>

#include "unixsocketipctransport.hpp"
#include "tcpipctransport.hpp"
#include "websocketipctransport.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser cmdParser;
    cmdParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    cmdParser.setApplicationDescription("Instant WebView is a scriptable WebView for developers.");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(QCommandLineOption(QStringList() << "t" << "transport", "IPC Transport Layer to use.", "tcp|unixsocket|websocket"));
    cmdParser.addPositionalArgument("command", "Command to execute. Pass \"-\" to read from stdin.");
    cmdParser.process(app.arguments());

    if (cmdParser.value("transport").isEmpty()) {
        qDebug().noquote() << "You must provide a IPC transport layer";
        return -1;
    }

    QStringList transportOptions =  cmdParser.value("transport").split(":");
    QString transportLayerName = transportOptions.first();

    IpcClient *client = NULL;

    if (transportLayerName == "unixsocket") {
        client = UnixSocketIpcClient::newClient(transportOptions);
    } else if (transportLayerName == "tcp") {
        client = TcpIpcClient::newClient(transportOptions);
    } else if (transportLayerName == "websocket") {
        client = WebSocketIpcClient::newClient(transportOptions);
    }

    QString command = cmdParser.positionalArguments().join(' ');

    if (command == "-") {
        QTextStream in(stdin, QIODevice::ReadOnly);
        if (!isatty(fileno(stdin)))
            command = in.readAll();
        else
            command.clear();
    }

    if (command.isEmpty()) {
        qDebug("No command provided");
        return 1;
    }

    QObject::connect(client, &IpcClient::connected, [client, command]() {
        client->write(command.toLocal8Bit());

        if (!command.startsWith('@')) {
            QTimer::singleShot(0, [=]() {
                QCoreApplication::exit(-1);
            });
        }
    });

    QObject::connect(client, &IpcClient::disconnected, [=]() {
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });
    });

    QObject::connect(client, &IpcClient::newData, [=](const QByteArray &data) {
        QTextStream out(stdout);
        out << data;
    });

    return app.exec();
}
