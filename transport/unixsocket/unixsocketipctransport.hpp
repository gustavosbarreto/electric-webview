#ifndef UNIXSOCKETIPCTRANSPORT_HPP
#define UNIXSOCKETIPCTRANSPORT_HPP

#include "ipctransportlayer.hpp"
#include "ipcclient.hpp"

class QLocalServer;
class QLocalSocket;

class UnixSocketIpcTransport: public IpcTransportLayer
{
public:
    UnixSocketIpcTransport();

    virtual void initialize();

    virtual void sendReply(QPointer<IpcClient> client, const QByteArray &data);

private:
    QLocalServer *m_server;
};

class UnixSocketIpcClient: public IpcClient
{
    Q_OBJECT

public:
    inline void setSocket(QLocalSocket *s) { m_socket = s; }

    virtual void initialize();
    virtual void close();
    virtual void write(const QByteArray &data);

    static IpcClient *newClient(const QStringList &args);

private:
    QLocalSocket *m_socket;
};

#endif // UNIXSOCKETIPCTRANSPORT_HPP
