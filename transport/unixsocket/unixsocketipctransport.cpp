#include "unixsocketipctransport.hpp"

#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QCoreApplication>

UnixSocketIpcTransport::UnixSocketIpcTransport()
{
}

void UnixSocketIpcTransport::initialize()
{
    QString name = m_options.value(1);

    if (name.isEmpty())
        name = "instant-webview";

    QLocalServer::removeServer(name);

    m_server = new QLocalServer;
    m_server->listen(name);

    if (m_server->isListening()) {
        qInfo().noquote() << QString("unixsocket: Listening on %1").arg(name);
    } else {
        qDebug().noquote() << QString("unixsocket: Failed to listen on %1").arg(name);
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });
    }

    connect(m_server, &QLocalServer::newConnection, [=]() {
        QLocalSocket *socket = m_server->nextPendingConnection();

        UnixSocketIpcClient *client = new UnixSocketIpcClient;
        client->setSocket(socket);
        client->initialize();

        connect(socket, &QLocalSocket::disconnected, [=]() {
            socket->deleteLater();
            client->deleteLater();
        });

        connect(client, &UnixSocketIpcClient::newData, [=](const QByteArray &data) {
            emit newData(client, data);
        });
    });
}

void UnixSocketIpcTransport::sendReply(QPointer<IpcClient> client, const QByteArray &data)
{
    UnixSocketIpcClient *unixSocketClient = qobject_cast<UnixSocketIpcClient *>(client);
    unixSocketClient->write(data);
}

void UnixSocketIpcClient::initialize()
{
    connect(m_socket, &QLocalSocket::readyRead, [=]() {
        emit newData(m_socket->readAll());
    });
}

void UnixSocketIpcClient::close()
{
    m_socket->close();
}

void UnixSocketIpcClient::write(const QByteArray &data)
{
    m_socket->write(data);
}

IpcClient *UnixSocketIpcClient::newClient(const QStringList &args)
{
    QVariantMap options;
    options["name"] = args.value(1);

    QLocalSocket *socket = new QLocalSocket();

    UnixSocketIpcClient *client = new UnixSocketIpcClient;
    client->setSocket(socket);
    client->initialize();

    connect(socket, &QLocalSocket::connected, [client, options]() {
        qInfo().noquote() << QString("unixsocket: Connected to %1").arg(options["name"].toString());
        emit client->connected();
    });

    connect(socket, &QLocalSocket::disconnected, [options]() {
        qInfo().noquote() << QString("unixsocket: Disconnected from %1").arg(options["name"].toString());
    });

    connect(socket, static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error), [options](QLocalSocket::LocalSocketError e) {
        Q_UNUSED(e);
        qDebug().noquote() << QString("unixsocket: Failed to connect to %1").arg(options["name"].toString());
    });

    socket->connectToServer(options["name"].toString());

    return client;
}
