#include "event.hpp"

Event::Event()
{
}

Event::Event(const Event &other)
{
    m_name = other.m_name;
    m_subscriptionCommand = other.m_subscriptionCommand;
}

QString Event::name() const
{
    return m_name;
}

void Event::setName(const QString &name)
{
    m_name = name;
}

Command Event::subscriptionCommand() const
{
    return m_subscriptionCommand;
}

void Event::setSubscriptionCommand(const Command &subscriptionCommand)
{
    m_subscriptionCommand = subscriptionCommand;
}

void Event::operator =(const Event &other)
{
    m_name = other.m_name;
    m_subscriptionCommand = other.m_subscriptionCommand;
}

bool Event::operator ==(const Event &other)
{
    return m_name == other.m_name && m_subscriptionCommand == other.m_subscriptionCommand;
}
