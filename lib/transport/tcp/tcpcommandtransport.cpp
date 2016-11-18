#include "tcpcommandtransport.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QCoreApplication>

TcpCommandTransport::TcpCommandTransport()
{
}

void TcpCommandTransport::initialize()
{
    QString host = m_options.value(1);
    QString port = m_options.value(2);

    if (host.isEmpty())
        host = "0.0.0.0";
    if (port.isEmpty())
        port = "38395";

    m_tcpServer = new QTcpServer(this);
    m_tcpServer->listen(QHostAddress(host), port.toInt());

    if (m_tcpServer->isListening()) {
        qInfo().noquote() << QString("tcp: Listening on %1:%2").arg(host).arg(port);
    } else {
        qDebug().noquote() << QString("tcp: Failed to listen on %1:%2").arg(host).arg(port);
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });
    }

    connect(m_tcpServer, &QTcpServer::newConnection, [=]() {
        QTcpSocket *socket = m_tcpServer->nextPendingConnection();

        TcpCommandClient *client = new TcpCommandClient;
        client->setSocket(socket);
        client->initialize();

        connect(socket, &QTcpSocket::disconnected, [=]() {
            socket->deleteLater();
            client->deleteLater();
        });

        connect(client, &TcpCommandClient::newData, [=](const QByteArray &data) {
            emit newData(client, data);
        });
    });
}

void TcpCommandTransport::sendReply(QPointer<CommandClient> client, const QByteArray &data)
{
    TcpCommandClient *tcpClient = qobject_cast<TcpCommandClient *>(client);
    tcpClient->write(data);
}

void TcpCommandClient::initialize()
{
    connect(m_socket, &QTcpSocket::readyRead, [=]() {
        emit newData(m_socket->readAll());
    });
}

void TcpCommandClient::close()
{
    m_socket->disconnectFromHost();
}

void TcpCommandClient::write(const QByteArray &data)
{
    m_socket->write(data);
}

CommandClient *TcpCommandClient::newClient(const QStringList &args, bool reverse)
{
    QVariantMap options;
    options["host"] = args.value(1);
    options["port"] = args.value(2);

    QTcpSocket *socket = new QTcpSocket();

    TcpCommandClient *client = new TcpCommandClient;
    client->setSocket(socket);
    client->initialize();

    connect(socket, &QTcpSocket::connected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("tcp: Connected to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
        emit client->connected();
    });

    connect(socket, &QTcpSocket::disconnected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("tcp: Disconnected from %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
        emit client->disconnected();
    });

    connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), [options, reverse](QAbstractSocket::SocketError e) {
        Q_UNUSED(e);
        if (reverse)
            qDebug().noquote() << QString("tcp: Failed to connect to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
    });

    QTimer::singleShot(0, [socket, options]() {
        socket->connectToHost(options["host"].toString(), options["port"].toUInt());
    });

    return client;
}

