#include "ipcserver.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

#include "tcpipctransport.hpp"
#include "unixsocketipctransport.hpp"
#include "websocketipctransport.hpp"

IpcServer::IpcServer(QObject *parent)
    : QObject(parent)
    , m_reverse(false)
    , m_transportLayer(NULL)
    , m_reverseTransport(NULL)
{
}

void IpcServer::initialize()
{
    QStringList transportOptions = m_transport.split(":");
    QString transportLayerName = transportOptions.first();
    QStringList supportedTransportLayers = QStringList()
                                           << "tcp"
                                           << "unixsocket"
                                           << "websocket";

    if (!supportedTransportLayers.contains(transportLayerName)) {
        qDebug().noquote() << "Invalid IPC transport layer";
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });

        return;
    }

    if (m_reverse) {
        if (transportLayerName == "tcp")
            m_reverseTransport = TcpIpcClient::newClient(transportOptions);
        else if (transportLayerName == "unixsocket")
            m_reverseTransport = UnixSocketIpcClient::newClient(transportOptions);
        else if (transportLayerName == "websocket")
            m_reverseTransport = WebSocketIpcClient::newClient(transportOptions);

        connect(m_reverseTransport, &IpcClient::connected, [=]() {
            // Send identification message to the server
            m_reverseTransport->write(QString("identify %1").arg(m_reverseId).toLocal8Bit());
        });

        connect(m_reverseTransport, &IpcClient::newData, [=](const QByteArray &data) {
            parseData(m_reverseTransport, data);
        });
    } else {
        if (transportLayerName == "tcp")
            m_transportLayer = new TcpIpcTransport;
        else if (transportLayerName == "unixsocket")
            m_transportLayer = new UnixSocketIpcTransport;
        else if (transportLayerName == "websocket")
            m_transportLayer = new WebSocketIpcTransport;

        m_transportLayer->setOptions(transportOptions);
        m_transportLayer->initialize();

        connect(m_transportLayer, &IpcTransportLayer::newData, [=](IpcClient *client, const QByteArray &data) {
            parseData(client, data);
        });
    }
}

void IpcServer::setReverse(bool reverse)
{
    m_reverse = reverse;
}

bool IpcServer::reverse()
{
    return m_reverse;
}

void IpcServer::setReverseId(const QString &id)
{
    m_reverseId = id;
}

void IpcServer::setTransport(const QString &transport)
{
    m_transport = transport;
}

void IpcServer::sendReply(QPointer<IpcClient> client, const QByteArray &data)
{
    m_transportLayer->sendReply(client, data);
}

void IpcServer::parseData(QPointer<IpcClient> client, const QByteArray &data)
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
        cmd.setSingleShot(args.first().startsWith('@'));

        if (cmd.isSingleShot())
            cmd.setName(cmd.name().mid(1));

        emit newCommand(cmd);
    }

    // A empty new line closes the client connection
    if (lines.mid(lines.size() - 2).first().isEmpty()) {
        if (!client.isNull())
            client->close();
    }
}
