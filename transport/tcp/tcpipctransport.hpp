#ifndef TCPIPCTRANSPORT_HPP
#define TCPIPCTRANSPORT_HPP

#include "ipctransportlayer.hpp"
#include "ipcclient.hpp"

class QTcpServer;
class QTcpSocket;

class TcpIpcTransport: public IpcTransportLayer
{
public:
    TcpIpcTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<IpcClient> client, const QByteArray &data);

private:
    QTcpServer *m_tcpServer;
};

class TcpIpcClient: public IpcClient
{
    Q_OBJECT

public:
    inline void setSocket(QTcpSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static IpcClient *newClient(const QStringList &args);

private:
    QTcpSocket *m_socket;
};

#endif // TCPIPCTRANSPORT_HPP
