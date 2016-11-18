#ifndef TCPCOMMANDTRANSPORT_HPP
#define TCPCOMMANDTRANSPORT_HPP

#include <command/commandtransportlayer.hpp>
#include <command/commandclient.hpp>

class QTcpServer;
class QTcpSocket;

class TcpCommandTransport: public CommandTransportLayer
{
public:
    TcpCommandTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<CommandClient> client, const QByteArray &data);

private:
    QTcpServer *m_tcpServer;
};

class TcpCommandClient: public CommandClient
{
    Q_OBJECT

public:
    inline void setSocket(QTcpSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static CommandClient *newClient(const QStringList &args, bool reverse = false);

private:
    QTcpSocket *m_socket;
};

#endif // TCPCOMMANDTRANSPORT_HPP
