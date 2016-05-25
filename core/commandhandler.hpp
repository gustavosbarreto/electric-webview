#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <QObject>
#include <QPointer>

#include "command.hpp"

class IpcClient;
class EventManager;

class CommandHandler: public QObject
{
    Q_OBJECT

public:
    CommandHandler(QObject *parent = 0);

    void processCommand(const Command &command) const;

    void setEventManager(EventManager *eventManager);

private:
    void processScreenshotCommand(const Command &command) const;
    void processJavaScriptCommand(const Command &command) const;

private:
    EventManager *m_eventManager;
};

#endif // COMMANDHANDLER_HPP
