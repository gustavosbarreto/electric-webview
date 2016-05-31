#ifndef WEBSOCKETIPCTRANSPORT_HPP
#define WEBSOCKETIPCTRANSPORT_HPP

#include "ipctransportlayer.hpp"
#include "ipcclient.hpp"

class QWebSocketServer;
class QWebSocket;

class WebSocketIpcTransport: public IpcTransportLayer
{
public:
    WebSocketIpcTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<IpcClient> client, const QByteArray &data);

private:
    QWebSocketServer *m_socketServer;
};

class WebSocketIpcClient: public IpcClient
{
    Q_OBJECT

public:
    inline void setSocket(QWebSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static IpcClient *newClient(const QStringList &args, bool reverse = false);

private:
    QWebSocket *m_socket;
};

#endif // WEBSOCKETIPCTRANSPORT_HPP
