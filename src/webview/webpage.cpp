#include "webpage.hpp"

#include <core/instantwebview.hpp>
#include <core/eventmanager.hpp>

WebPage::WebPage(QObject *parent)
    : QWebEnginePage(parent)
{
}

void WebPage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    Q_UNUSED(lineNumber);
    Q_UNUSED(sourceID);

    QString eventName;

    if (level == QWebEnginePage::InfoMessageLevel)
        eventName = "info_message_raised";
    else if (level == QWebEnginePage::WarningMessageLevel)
        eventName = "warning_message_raised";
    else if (level == QWebEnginePage::ErrorMessageLevel)
        eventName = "error_message_raised";

    const QList<Event> &subscribers = InstantWebView::instance()->eventManager()->subscribers(eventName);
    foreach (const Event &event, subscribers) {
        event.sendResponse(message.toLocal8Bit());
    }
}
