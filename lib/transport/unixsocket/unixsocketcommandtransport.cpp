#include "unixsocketcommandtransport.hpp"

#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>

UnixSocketCommandTransport::UnixSocketCommandTransport()
{
}

void UnixSocketCommandTransport::initialize()
{
    QString name = m_options.value(1);
    QFileInfo fileInfo(name);

    if (!fileInfo.isAbsolute())
        name = name.prepend(QDir::currentPath() + "/");

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

        UnixSocketCommandClient *client = new UnixSocketCommandClient;
        client->setSocket(socket);
        client->initialize();

        connect(socket, &QLocalSocket::disconnected, [=]() {
            socket->deleteLater();
            client->deleteLater();
        });

        connect(client, &UnixSocketCommandClient::newData, [=](const QByteArray &data) {
            emit newData(client, data);
        });
    });
}

void UnixSocketCommandTransport::sendReply(QPointer<CommandClient> client, const QByteArray &data)
{
    UnixSocketCommandClient *unixSocketClient = qobject_cast<UnixSocketCommandClient *>(client);
    unixSocketClient->write(data);
}

void UnixSocketCommandClient::initialize()
{
    connect(m_socket, &QLocalSocket::readyRead, [=]() {
        emit newData(m_socket->readAll());
    });
}

void UnixSocketCommandClient::close()
{
    m_socket->disconnectFromServer();
}

void UnixSocketCommandClient::write(const QByteArray &data)
{
    m_socket->write(data);
}

CommandClient *UnixSocketCommandClient::newClient(const QStringList &args, bool reverse)
{
    QVariantMap options;
    options["name"] = args.value(1);

    QLocalSocket *socket = new QLocalSocket();

    UnixSocketCommandClient *client = new UnixSocketCommandClient;
    client->setSocket(socket);
    client->initialize();

    connect(socket, &QLocalSocket::connected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("unixsocket: Connected to %1").arg(options["name"].toString());
        emit client->connected();
    });

    connect(socket, &QLocalSocket::disconnected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("unixsocket: Disconnected from %1").arg(options["name"].toString());
        emit client->disconnected();
    });

    connect(socket, static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error), [options, reverse](QLocalSocket::LocalSocketError e) {
        Q_UNUSED(e);
        if (reverse)
            qDebug().noquote() << QString("unixsocket: Failed to connect to %1").arg(options["name"].toString());
    });

    QTimer::singleShot(0, [socket, options]() {
       socket->connectToServer(options["name"].toString());
    });

    return client;
}
