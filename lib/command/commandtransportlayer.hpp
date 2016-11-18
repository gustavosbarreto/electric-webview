#ifndef COMMANDTRANSPORTLAYER_HPP
#define COMMANDTRANSPORTLAYER_HPP

#include <QObject>
#include <QPointer>

class CommandClient;

class CommandTransportLayer: public QObject
{
    Q_OBJECT

public:
    CommandTransportLayer(QObject *parent = 0);

    virtual void initialize() = 0;

    void setOptions(const QStringList &options);

    virtual void sendReply(QPointer<CommandClient> client, const QByteArray &data) = 0;

signals:
    void newData(QPointer<CommandClient> client, const QByteArray &data);

protected:
    QStringList m_options;
};

#endif // COMMANDTRANSPORTLAYER_HPP
