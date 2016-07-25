#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <QObject>
#include <QPointer>

#include <ipc/ipcclient.hpp>

class Command
{
public:
    Command();
    Command(const Command &other);

    void setName(const QString &name);
    QString name() const;

    void setArguments(const QStringList &arguments);
    QStringList arguments() const;

    void setGetter(bool getter);
    bool isGetter() const;

    void setClient(QPointer<IpcClient> client);
    QPointer<IpcClient> client() const;

    void sendResponse(const QByteArray &data) const;

    void operator =(const Command &other);
    bool operator ==(const Command &other);

private:
    QString m_name;
    QStringList m_arguments;
    bool m_getter;
    QPointer<IpcClient> m_client;
};

#endif // COMMAND_HPP
