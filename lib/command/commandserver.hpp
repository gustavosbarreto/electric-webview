#ifndef COMMANDSERVER_HPP
#define COMMANDSERVER_HPP

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QPointer>

#include "commandclient.hpp"

#include <core/command.hpp>

class QTcpServer;
class QTcpSocket;

class CommandTransportLayer;

class CommandServer: public QObject
{
    Q_OBJECT

public:
    CommandServer(QObject *parent = 0);

    void initialize();

    void setReverse(bool reverse);
    bool reverse();
    void setReverseId(const QString &id);
    void setTransport(const QString &transport);

    void sendReply(QPointer<CommandClient> client, const QByteArray &data);

signals:
    void newCommand(const Command &command);

private:
    void parseData(QPointer<CommandClient> client, const QByteArray &data);

private:
    bool m_reverse;
    QString m_reverseId;
    QString m_transport;
    CommandTransportLayer *m_transportLayer;
    CommandClient *m_reverseTransport;
};

#endif // COMMANDSERVER_HPP
