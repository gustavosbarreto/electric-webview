#ifndef IPCCLIENT_HPP
#define IPCCLIENT_HPP

#include <QObject>
#include <QByteArray>

class IpcClient: public QObject
{
    Q_OBJECT

public:
    IpcClient(QObject *parent = 0);

    virtual void initialize();
    virtual void close() = 0;
    virtual void write(const QByteArray &data) = 0;

    static IpcClient *newClient(const QStringList &args);

signals:
    void connected();
    void newData(const QByteArray &data);
};

#endif // IPCCLIENT_HPP
