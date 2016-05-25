#ifndef EVENT_HPP
#define EVENT_HPP

#include <QObject>

#include "command.hpp"

class Event
{
public:
    Event();
    Event(const Event &other);

    QString name() const;
    void setName(const QString &name);

    Command subscriptionCommand() const;
    void setSubscriptionCommand(const Command &subscriptionCommand);

    void operator =(const Event &other);
    bool operator ==(const Event &other);

private:
    QString m_name;
    Command m_subscriptionCommand;
};

#endif // EVENT_HPP
