#include "command.hpp"

Command::Command()
    : m_getter(false)
{
}

Command::Command(const Command &other)
{
    m_name = other.m_name;
    m_arguments = other.m_arguments;
    m_getter = other.m_getter;
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

void Command::setGetter(bool getter)
{
    m_getter = getter;
}

bool Command::isGetter() const
{
    return m_getter;
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

    QByteArray prefix;

    if (!m_getter)
        prefix = m_name.toLocal8Bit() + " ";

    m_client->write(prefix + data + "\n");

    if (m_getter)
        m_client->close();
}

void Command::operator =(const Command &other)
{
    m_name = other.m_name;
    m_arguments = other.m_arguments;
    m_getter = other.m_getter;
    m_client = other.m_client;
}

bool Command::operator ==(const Command &other)
{
    return m_name == other.m_name && m_arguments == other.m_arguments && m_getter == other.m_getter && m_client == other.m_client;
}
