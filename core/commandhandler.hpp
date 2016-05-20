#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <QObject>
#include <QPointer>

class IpcClient;
class EventManager;

class CommandHandler: public QObject
{
    Q_OBJECT

public:
    CommandHandler(QObject *parent = 0);

    void processCommand(QPointer<IpcClient> client, QString command, QStringList args);

    void setEventManager(EventManager *eventManager);

private:
    void processJavaScriptCommand(QPointer<IpcClient> client, QStringList args);

private:
    EventManager *m_eventManager;
};

#endif // COMMANDHANDLER_HPP
