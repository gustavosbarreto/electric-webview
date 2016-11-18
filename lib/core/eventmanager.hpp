#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <QObject>
#include <QMap>
#include <QList>

#include "event.hpp"

class QWebEngineView;

class CommandClient;

class EventManager: public QObject
{
    Q_OBJECT

public:
    EventManager(QObject *parent = 0);

    void bind();
    void subscribe(const Event &event);

    const QList<Event> &subscribers(const QString &eventName);

private:
    QWebEngineView *m_webView;
    QMap<QString, QList<Event> > m_subscribers;
};

#endif // EVENTMANAGER_HPP
