#ifndef IPCSERVER_HPP
#define IPCSERVER_HPP

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QPointer>

#include "ipcclient.hpp"
#include "command.hpp"

class QTcpServer;
class QTcpSocket;

class IpcTransportLayer;

class IpcServer: public QObject
{
    Q_OBJECT

public:
    IpcServer(QObject *parent = 0);

    void initialize();

    void setReverse(bool reverse);
    bool reverse();
    void setReverseId(const QString &id);
    void setTransport(const QString &transport);

    void sendReply(QPointer<IpcClient> client, const QByteArray &data);

signals:
    void newCommand(const Command &command);

private:
    void parseData(QPointer<IpcClient> client, const QByteArray &data);

private:
    bool m_reverse;
    QString m_reverseId;
    QString m_transport;
    IpcTransportLayer *m_transportLayer;
    IpcClient *m_reverseTransport;
};

#endif // IPCSERVER_HPP
