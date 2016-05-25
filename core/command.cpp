#include "command.hpp"

Command::Command()
    : m_singleShot(false)
{
}

Command::Command(const Command &other)
{
    m_name = other.m_name;
    m_arguments = other.m_arguments;
    m_singleShot = other.m_singleShot;
    m_client = other.m_client;
}

void Command::setName(const QString &name)
{
    m_name = name;
}

QString Command::name() const
{
    return m_name;
}

void Command::setArguments(const QStringList &arguments)
{
    m_arguments = arguments;
}

QStringList Command::arguments() const
{
    return m_arguments;
}

void Command::setSingleShot(bool singleShot)
{
    m_singleShot = singleShot;
}

bool Command::isSingleShot() const
{
    return m_singleShot;
}

void Command::setClient(QPointer<IpcClient> client)
{
    m_client = client;
}

QPointer<IpcClient> Command::client() const
{
    return m_client;
}

void Command::sendResponse(const QByteArray &data) const
{
    if (m_client.isNull())
        return;

    m_client->write(data + "\n");

    if (m_singleShot)
        m_client->close();
}

void Command::operator =(const Command &other)
{
    m_name = other.m_name;
    m_arguments = other.m_arguments;
    m_singleShot = other.m_singleShot;
    m_client = other.m_client;
}

bool Command::operator ==(const Command &other)
{
    return m_name == other.m_name && m_arguments == other.m_arguments && m_singleShot == other.m_singleShot && m_client == other.m_client;
}
