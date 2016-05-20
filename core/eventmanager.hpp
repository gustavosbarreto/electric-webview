#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <QObject>
#include <QMap>
#include <QList>

class QWebEngineView;

class IpcClient;

class EventManager: public QObject
{
    Q_OBJECT

public:
    EventManager(QObject *parent = 0);

    void bind();
    void subscribe(const QString &event, IpcClient *client);

    QWebEngineView *webView();
    void setWebView(QWebEngineView *view);

private:
    QWebEngineView *m_webView;
    QMap<QString, QList<IpcClient *> > m_subscribers;
};

#endif // EVENTMANAGER_HPP
