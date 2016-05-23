#include "tcpipctransport.hpp"

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QCoreApplication>

TcpIpcTransport::TcpIpcTransport()
{
}

void TcpIpcTransport::initialize()
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

        TcpIpcClient *client = new TcpIpcClient;
        client->setSocket(socket);
        client->initialize();

        connect(socket, &QTcpSocket::disconnected, [=]() {
            socket->deleteLater();
            client->deleteLater();
        });

        connect(client, &TcpIpcClient::newData, [=](const QByteArray &data) {
            emit newData(client, data);
        });
    });
}

void TcpIpcTransport::sendReply(QPointer<IpcClient> client, const QByteArray &data)
{
    TcpIpcClient *tcpIpcClient = qobject_cast<TcpIpcClient *>(client);
    tcpIpcClient->write(data);
}

void TcpIpcClient::initialize()
{
    connect(m_socket, &QTcpSocket::readyRead, [=]() {
        emit newData(m_socket->readAll());
    });
}

void TcpIpcClient::close()
{
    m_socket->close();
}

void TcpIpcClient::write(const QByteArray &data)
{
    m_socket->write(data);
}

IpcClient *TcpIpcClient::newClient(const QStringList &args)
{
    QVariantMap options;
    options["host"] = args.value(1);
    options["port"] = args.value(2);

    QTcpSocket *socket = new QTcpSocket();

    TcpIpcClient *client = new TcpIpcClient;
    client->setSocket(socket);
    client->initialize();

    connect(socket, &QTcpSocket::connected, [client, options]() {
        qInfo().noquote() << QString("tcp: Connected to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
        emit client->connected();
    });

    connect(socket, &QTcpSocket::disconnected, [options]() {
        qInfo().noquote() << QString("tcp: Disconnected from %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
    });

    connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), [options](QAbstractSocket::SocketError e) {
        Q_UNUSED(e);
        qDebug().noquote() << QString("tcp: Failed to connect to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
    });

    socket->connectToHost(options["host"].toString(), options["port"].toUInt());

    return client;
}

