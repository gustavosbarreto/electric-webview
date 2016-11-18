#ifndef WEBSOCKETCOMMANDTRANSPORT_HPP
#define WEBSOCKETCOMMANDTRANSPORT_HPP

#include <command/commandtransportlayer.hpp>
#include <command/commandclient.hpp>

class QWebSocketServer;
class QWebSocket;

class WebSocketCommandTransport: public CommandTransportLayer
{
public:
    WebSocketCommandTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<CommandClient> client, const QByteArray &data);

private:
    QWebSocketServer *m_socketServer;
};

class WebSocketCommandClient: public CommandClient
{
    Q_OBJECT

public:
    inline void setSocket(QWebSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static CommandClient *newClient(const QStringList &args, bool reverse = false);

private:
    QWebSocket *m_socket;
};

#endif // WEBSOCKETCOMMANDTRANSPORT_HPP
