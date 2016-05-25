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
        QStringList events = QStringList() << "url_changed" << "@url_changed";

        foreach (const QString &event, events) {
            foreach (IpcClient *client, m_subscribers[event]) {
                bool isSingleShot = event.startsWith('@');

                if (isSingleShot) {
                    client->write(QString("%1\n").arg(url.toString()).toUtf8());
                    client->close();
                } else {
                    client->write(QString("url_changed %1\n").arg(url.toString()).toUtf8());
                }
            }
        }
    });

    // title_changed EVENT
    connect(m_webView, &QWebEngineView::titleChanged, [=](const QString &title) {
        QStringList events = QStringList() << "title_changed" << "@title_changed";

        foreach (const QString &event, events) {
            foreach (IpcClient *client, m_subscribers[event]) {
                bool isSingleShot = event.startsWith('@');

                if (isSingleShot) {
                    client->write(QString("%1\n").arg(title).toUtf8());
                    client->close();
                } else {
                    client->write(QString("title_changed %1\n").arg(title).toUtf8());
                }
            }
        }
    });

    // load_started EVENT
    connect(m_webView, &QWebEngineView::loadStarted, [=]() {
        QStringList events = QStringList() << "load_started" << "@load_started";

        foreach (const QString &event, events) {
            foreach (IpcClient *client, m_subscribers[event]) {
                bool isSingleShot = event.startsWith('@');

                if (isSingleShot) {
                    client->write(QString("\n").toUtf8());
                    client->close();
                } else {
                    client->write(QString("load_started\n").toUtf8());
                }
            }
        }
    });

    // load_finished EVENT
    connect(m_webView, &QWebEngineView::loadFinished, [=](bool ok) {
        QStringList events = QStringList() << "load_finished" << "@load_finished";

        foreach (const QString &event, events) {
            foreach (IpcClient *client, m_subscribers[event]) {
                bool isSingleShot = event.startsWith('@');

                if (isSingleShot) {
                    client->write(QString("%1\n").arg(ok).toUtf8());
                    client->close();
                } else {
                    client->write(QString("load_finished %1\n").arg(ok).toUtf8());
                }
            }
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
