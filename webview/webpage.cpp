#include "webpage.hpp"

#include "instantwebview.hpp"
#include "eventmanager.hpp"

WebPage::WebPage(QObject *parent)
    : QWebEnginePage(parent)
{
}

void WebPage::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    Q_UNUSED(lineNumber);
    Q_UNUSED(sourceID);

    const QList<Event> &subscribers = InstantWebView::instance()->eventManager()->subscribers("console_message");
    foreach (const Event &event, subscribers) {
        QStringList response;

        if (level == QWebEnginePage::InfoMessageLevel)
            response.append("info");
        else if (level == QWebEnginePage::WarningMessageLevel)
            response.append("warning");
        else if (level == QWebEnginePage::ErrorMessageLevel)
            response.append("error");

        response.append("\"" + QString(message).replace("\"", "\\\"") + "\"");

        event.sendResponse(response.join(' ').toLocal8Bit());
    }
}
