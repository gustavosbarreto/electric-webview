#ifndef COMMANDCLIENT_HPP
#define COMMANDCLIENT_HPP

#include <QObject>
#include <QByteArray>

class CommandClient: public QObject
{
    Q_OBJECT

public:
    CommandClient(QObject *parent = 0);

    virtual void initialize();
    virtual void close() = 0;
    virtual void write(const QByteArray &data) = 0;

    static CommandClient *newClient(const QStringList &args, bool reverse = false);

protected:

signals:
    void connected();
    void disconnected();
    void newData(const QByteArray &data);
};

#endif // COMMANDCLIENT_HPP
