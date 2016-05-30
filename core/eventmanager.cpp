#include "eventmanager.hpp"

#include <QWebEngineView>

#include "instantwebview.hpp"
#include "ipcclient.hpp"
#include "inputeventfilter.hpp"

EventManager::EventManager(QObject *parent)
    : QObject(parent)
{
}

void EventManager::bind()
{
    QWebEngineView *webView = InstantWebView::instance()->webView();

    // url_changed EVENT
    connect(webView, &QWebEngineView::urlChanged, [=](const QUrl &url) {
        foreach (const Event &event, m_subscribers["url_changed"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(url.toString()).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("url_changed %1").arg(url.toString()).toUtf8());
            }
        }
    });

    // title_changed EVENT
    connect(webView, &QWebEngineView::titleChanged, [=](const QString &title) {
        foreach (const Event &event, m_subscribers["title_changed"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(title).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("title_changed %1").arg(title).toUtf8());
            }
        }
    });

    // load_started EVENT
    connect(webView, &QWebEngineView::loadStarted, [=]() {
        foreach (const Event &event, m_subscribers["load_started"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("").toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("load_started").toUtf8());
            }
        }
    });

    // load_finished EVENT
    connect(webView, &QWebEngineView::loadFinished, [=](bool ok) {
        foreach (const Event &event, m_subscribers["load_finished"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(ok).toUtf8());
            } else {
                event.subscriptionCommand().sendResponse(QString("load_finished %1").arg(ok).toUtf8());
            }
        }
    });

    // user_activity EVENT
    connect(InstantWebView::instance()->inputEventFilter(), &InputEventFilter::activity, [=](int idleTime) {
        foreach (const Event &event, m_subscribers["user_activity"]) {
            if (event.subscriptionCommand().isSingleShot()) {
                event.subscriptionCommand().sendResponse(QString("%1").arg(idleTime).toUtf8());
                m_subscribers[event.name()].removeOne(event);
            } else {
                event.subscriptionCommand().sendResponse(QString("user_activity %1").arg(idleTime).toUtf8());
            }
        }
    });
}

void EventManager::subscribe(const Event &event)
{
    m_subscribers[event.name()].append(event);

    // Unsubscribe
    connect(event.subscriptionCommand().client(), &QObject::destroyed, [this, event]() {
        m_subscribers[event.name()].removeOne(event);
    });
}
