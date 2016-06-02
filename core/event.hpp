#ifndef EVENT_HPP
#define EVENT_HPP

#include <QObject>

#include "command.hpp"

class Event: public Command
{
public:
    Event(const Command &command);
};

#endif // EVENT_HPP
