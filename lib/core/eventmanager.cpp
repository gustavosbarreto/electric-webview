#include "eventmanager.hpp"

#include <QWebEngineView>

#include "electricwebview.hpp"
#include "inputeventfilter.hpp"

#include <ipc/ipcclient.hpp>

EventManager::EventManager(QObject *parent)
    : QObject(parent)
{
}

void EventManager::bind()
{
    QWebEngineView *webView = ElectricWebView::instance()->webView();

    // url_changed EVENT
    connect(webView, &QWebEngineView::urlChanged, [=](const QUrl &url) {
        foreach (const Event &event, m_subscribers["url_changed"]) {
            event.sendResponse(QString("%1").arg(url.toString()).toUtf8());
        }
    });

    // title_changed EVENT
    connect(webView, &QWebEngineView::titleChanged, [=](const QString &title) {
        foreach (const Event &event, m_subscribers["title_changed"]) {
            event.sendResponse(QString("%1").arg(title).toUtf8());
        }
    });

    // load_started EVENT
    connect(webView, &QWebEngineView::loadStarted, [=]() {
        foreach (const Event &event, m_subscribers["load_started"]) {
            event.sendResponse(QString("").toUtf8());
        }
    });

    // load_finished EVENT
    connect(webView, &QWebEngineView::loadFinished, [=](bool ok) {
        foreach (const Event &event, m_subscribers["load_finished"]) {
            event.sendResponse(QString("%1").arg(ok).toUtf8());
        }
    });

    // user_activity EVENT
    connect(ElectricWebView::instance()->inputEventFilter(), &InputEventFilter::activity, [=](int idleTime) {
        foreach (const Event &event, m_subscribers["user_activity"]) {
            event.sendResponse(QString("%1").arg(idleTime).toUtf8());
            if (event.isSingleShot())
                m_subscribers[event.name()].removeOne(event);
        }
    });
}

void EventManager::subscribe(const Event &event)
{
    m_subscribers[event.name()].append(event);

    // Unsubscribe
    connect(event.client(), &QObject::destroyed, [this, event]() {
        m_subscribers[event.name()].removeOne(event);
    });
}

const QList<Event> &EventManager::subscribers(const QString &eventName)
{
    return m_subscribers[eventName];
}
