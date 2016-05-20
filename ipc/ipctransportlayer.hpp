#ifndef IPCTRANSPORTLAYER_HPP
#define IPCTRANSPORTLAYER_HPP

#include <QObject>
#include <QPointer>

class IpcClient;

class IpcTransportLayer: public QObject
{
    Q_OBJECT

public:
    IpcTransportLayer(QObject *parent = 0);

    virtual void initialize() = 0;

    void setOptions(const QStringList &options);

    virtual void sendReply(QPointer<IpcClient> client, const QByteArray &data) = 0;

signals:
    void newData(QPointer<IpcClient> client, const QByteArray &data);

protected:
    QStringList m_options;
};

#endif // IPCTRANSPORTLAYER_HPP
