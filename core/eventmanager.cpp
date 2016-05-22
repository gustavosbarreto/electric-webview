#include "eventmanager.hpp"

#include <QWebEngineView>

#include "ipcclient.hpp"

EventManager::EventManager(QObject *parent)
    : QObject(parent)
{
}

void EventManager::bind()
{
    // url_changed EVENT
    connect(m_webView, &QWebEngineView::urlChanged, [=](const QUrl &url) {
        foreach (IpcClient *client, m_subscribers["url_changed"]) {
            client->write(QString("url_changed %1\n").arg(url.toString()).toUtf8());
        }
    });

    // title_changed EVENT
    connect(m_webView, &QWebEngineView::titleChanged, [=](const QString &title) {
        foreach (IpcClient *client, m_subscribers["title_changed"]) {
            client->write(QString("title_changed %1\n").arg(title).toUtf8());
        }
    });

    // load_started EVENT
    connect(m_webView, &QWebEngineView::loadStarted, [=]() {
        foreach (IpcClient *client, m_subscribers["load_started"]) {
            client->write("load_started\n");
        }
    });

    // load_finished EVENT
    connect(m_webView, &QWebEngineView::loadFinished, [=](bool ok) {
        foreach (IpcClient *client, m_subscribers["load_finished"]) {
            client->write(QString("load_finished %1\n").arg(ok).toUtf8());
        }
    });
}

void EventManager::subscribe(const QString &event, IpcClient *client)
{
    m_subscribers[event].append(client);

    // Unsubscribe
    connect(client, &QObject::destroyed, [=]() {
       m_subscribers[event].removeOne(client);
    });
}

QWebEngineView *EventManager::webView()
{
    return m_webView;
}

void EventManager::setWebView(QWebEngineView *webView)
{
    m_webView = webView;
}
