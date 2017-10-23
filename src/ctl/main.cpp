#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <QSocketNotifier>
#include <QTextStream>
#include <QDebug>
#include <QFile>

#ifdef _WIN64
#include <io.h>
#else
#include <unistd.h>
#endif

#include <transport/unixsocket/unixsocketcommandtransport.hpp>
#include <transport/tcp/tcpcommandtransport.hpp>
#include <transport/websocket/websocketcommandtransport.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser cmdParser;
    cmdParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    cmdParser.setApplicationDescription("Electric WebView is a scriptable WebView for developers.");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(QCommandLineOption(QStringList() << "t" << "transport", "Command Transport Layer to use.", "tcp|unixsocket|websocket"));
    cmdParser.addPositionalArgument("command", "Command to execute. Pass \"-\" to read from stdin.");
    cmdParser.process(app.arguments());

    if (cmdParser.value("transport").isEmpty()) {
        qDebug().noquote() << "You must provide a command transport layer";
        return -1;
    }

    QStringList transportOptions =  cmdParser.value("transport").split(":");
    QString transportLayerName = transportOptions.first();

    CommandClient *client = NULL;

    if (transportLayerName == "unixsocket") {
        client = UnixSocketCommandClient::newClient(transportOptions);
    } else if (transportLayerName == "tcp") {
        client = TcpCommandClient::newClient(transportOptions);
    } else if (transportLayerName == "websocket") {
        client = WebSocketCommandClient::newClient(transportOptions);
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

    QObject::connect(client, &CommandClient::connected, [client, command]() {
        client->write(command.toLocal8Bit());

        if (!command.startsWith('@')) {
            QTimer::singleShot(0, [=]() {
                QCoreApplication::exit(-1);
            });
        }
    });

    QObject::connect(client, &CommandClient::disconnected, [=]() {
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });
    });

    QObject::connect(client, &CommandClient::newData, [=](const QByteArray &data) {
        QTextStream out(stdout);
        out << data;
    });

    return app.exec();
}
