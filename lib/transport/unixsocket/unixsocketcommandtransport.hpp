#ifndef UNIXSOCKETCOMMANDTRANSPORT_HPP
#define UNIXSOCKETCOMMANDTRANSPORT_HPP

#include <command/commandtransportlayer.hpp>
#include <command/commandclient.hpp>

class QLocalServer;
class QLocalSocket;

class UnixSocketCommandTransport: public CommandTransportLayer
{
public:
    UnixSocketCommandTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<CommandClient> client, const QByteArray &data);

private:
    QLocalServer *m_server;
};

class UnixSocketCommandClient: public CommandClient
{
    Q_OBJECT

public:
    inline void setSocket(QLocalSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static CommandClient *newClient(const QStringList &args, bool reverse = false);

private:
    QLocalSocket *m_socket;
};

#endif // UNIXSOCKETCOMMANDTRANSPORT_HPP
