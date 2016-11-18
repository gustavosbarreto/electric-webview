#include "commandserver.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include <transport/tcp/tcpcommandtransport.hpp>
#include <transport/unixsocket/unixsocketcommandtransport.hpp>
#include <transport/websocket/websocketcommandtransport.hpp>

CommandServer::CommandServer(QObject *parent)
    : QObject(parent)
    , m_reverse(false)
    , m_transportLayer(NULL)
    , m_reverseTransport(NULL)
{
}

void CommandServer::initialize()
{
    QStringList transportOptions = m_transport.split(":");
    QString transportLayerName = transportOptions.first();
    QStringList supportedTransportLayers = QStringList()
                                           << "tcp"
                                           << "unixsocket"
                                           << "websocket";

    if (!supportedTransportLayers.contains(transportLayerName)) {
        qDebug().noquote() << "Invalid command transport layer";
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });

        return;
    }

    if (m_reverse) {
        if (transportLayerName == "tcp")
            m_reverseTransport = TcpCommandClient::newClient(transportOptions);
        else if (transportLayerName == "unixsocket")
            m_reverseTransport = UnixSocketCommandClient::newClient(transportOptions);
        else if (transportLayerName == "websocket")
            m_reverseTransport = WebSocketCommandClient::newClient(transportOptions);

        connect(m_reverseTransport, &CommandClient::connected, [=]() {
            // Send identification message to the server
            m_reverseTransport->write(QString("identify %1").arg(m_reverseId).toLocal8Bit());
        });

        connect(m_reverseTransport, &CommandClient::newData, [=](const QByteArray &data) {
            parseData(m_reverseTransport, data);
        });
    } else {
        if (transportLayerName == "tcp")
            m_transportLayer = new TcpCommandTransport;
        else if (transportLayerName == "unixsocket")
            m_transportLayer = new UnixSocketCommandTransport;
        else if (transportLayerName == "websocket")
            m_transportLayer = new WebSocketCommandTransport;

        m_transportLayer->setOptions(transportOptions);
        m_transportLayer->initialize();

        connect(m_transportLayer, &CommandTransportLayer::newData, [=](CommandClient *client, const QByteArray &data) {
            parseData(client, data);
        });
    }
}

void CommandServer::setReverse(bool reverse)
{
    m_reverse = reverse;
}

bool CommandServer::reverse()
{
    return m_reverse;
}

void CommandServer::setReverseId(const QString &id)
{
    m_reverseId = id;
}

void CommandServer::setTransport(const QString &transport)
{
    m_transport = transport;
}

void CommandServer::sendReply(QPointer<CommandClient> client, const QByteArray &data)
{
    m_transportLayer->sendReply(client, data);
}

void CommandServer::parseData(QPointer<CommandClient> client, const QByteArray &data)
{
    QStringList lines = QString::fromLocal8Bit(data).split('\n');

    foreach (const QString &line, lines) {
        if (line.isEmpty())
            continue;

        QStringList args = line.split(' ');

        Command cmd;
        cmd.setName(args.first());
        cmd.setArguments(args.mid(1, -1));
        cmd.setClient(client);
        cmd.setGetter(args.first().startsWith('@'));

        if (cmd.isGetter())
            cmd.setName(cmd.name().mid(1));

        emit newCommand(cmd);
    }

    // A empty new line closes the client connection
    if (lines.mid(lines.size() - 2).first().isEmpty()) {
        if (!client.isNull())
            client->close();
    }
}
