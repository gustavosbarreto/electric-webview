#include "websocketipctransport.hpp"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
#include <QCoreApplication>

WebSocketIpcTransport::WebSocketIpcTransport()
{
}

void WebSocketIpcTransport::initialize()
{
    QString host = m_options.value(1);
    QString port = m_options.value(2);

    if (host.isEmpty())
        host = "0.0.0.0";
    if (port.isEmpty())
        port = "38395";

    m_socketServer = new QWebSocketServer("instant-webview", QWebSocketServer::NonSecureMode);
    m_socketServer->listen(QHostAddress(host), port.toInt());

    if (m_socketServer->isListening()) {
        qInfo().noquote() << QString("websocket: Listening on %1:%2").arg(host).arg(port);
    } else {
        qDebug().noquote() << QString("websocket: Failed to listen on %1:%2").arg(host).arg(port);
        QTimer::singleShot(0, [=]() {
            QCoreApplication::exit(-1);
        });
    }

    connect(m_socketServer, &QWebSocketServer::newConnection, [=]() {
        QWebSocket *socket = m_socketServer->nextPendingConnection();

        WebSocketIpcClient *client = new WebSocketIpcClient;
        client->setSocket(socket);
        client->initialize();

        connect(socket, &QWebSocket::disconnected, [=]() {
            socket->deleteLater();
            client->deleteLater();
        });

        connect(client, &WebSocketIpcClient::newData, [=](const QByteArray &data) {
            emit newData(client, data);
        });
    });
}

void WebSocketIpcTransport::sendReply(QPointer<IpcClient> client, const QByteArray &data)
{
    WebSocketIpcClient *webSocketIpcClient = qobject_cast<WebSocketIpcClient *>(client);
    webSocketIpcClient->write(data);
}

void WebSocketIpcClient::initialize()
{
    connect(m_socket, &QWebSocket::textMessageReceived, [=](const QString &message) {
        emit newData(message.toLocal8Bit());
    });
}

void WebSocketIpcClient::close()
{
    m_socket->close();
}

void WebSocketIpcClient::write(const QByteArray &data)
{
    m_socket->sendTextMessage(data);
}

IpcClient *WebSocketIpcClient::newClient(const QStringList &args, bool reverse)
{
    QVariantMap options;
    options["host"] = args.value(1);
    options["port"] = args.value(2);

    QWebSocket *socket = new QWebSocket();

    WebSocketIpcClient *client = new WebSocketIpcClient;
    client->setSocket(socket);
    client->initialize();

    connect(socket, &QWebSocket::connected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("websocket: Connected to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
        emit client->connected();
    });

    connect(socket, &QWebSocket::disconnected, [client, options, reverse]() {
        if (reverse)
            qInfo().noquote() << QString("websocket: Disconnected from %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
        emit client->disconnected();
    });

    connect(socket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), [options, reverse](QAbstractSocket::SocketError e) {
        Q_UNUSED(e);
        if (reverse)
            qDebug().noquote() << QString("websocket: Failed to connect to %1:%2").arg(options["host"].toString()).arg(options["port"].toString());
    });

    QTimer::singleShot(0, [socket, options]() {
        socket->open(QUrl(QString("ws://%1:%2").arg(options["host"].toString()).arg(options["port"].toString())));
    });

    return client;
}

