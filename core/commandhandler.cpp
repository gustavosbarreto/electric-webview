#include "commandhandler.hpp"

#include <QWebEngineView>
#include <QFile>
#include <QEventLoop>

#include "ipcclient.hpp"
#include "eventmanager.hpp"

CommandHandler::CommandHandler(QObject *parent)
    : QObject(parent)
{
}

void CommandHandler::processCommand(QPointer<IpcClient> client, QString command, QStringList args)
{
    if (command == "load") {
        if (args.isEmpty())
            m_eventManager->webView()->load(QUrl("about:blank"));
        else
            m_eventManager->webView()->load(QUrl(args.first()));
    } else if (command == "current_url") {
        client->write(m_eventManager->webView()->url().toString().toLocal8Bit());
        client->write("\n");
    } else if (command == "set_html") {
        QString type = args.value(0);
        QString value = args.value(1);

        if (type == "string") {
            m_eventManager->webView()->page()->setHtml(value.toLocal8Bit());
        } else if (type == "file") {
            QFile file(value);
            file.open(QFile::ReadOnly);

            m_eventManager->webView()->page()->setHtml(file.readAll());
        }
    } else if (command == "current_title") {
        client->write(m_eventManager->webView()->title().toLocal8Bit());
        client->write("\n");
    } else if (command == "subscribe") {
        QString event = args.first();
        QStringList events = QStringList()
                << "title_changed"
                << "url_changed"
                << "load_finished";

        if (events.contains(event))
            m_eventManager->subscribe(event, client);
    } else if (command == "js") {
        processJavaScriptCommand(client, args);
    }
}

void CommandHandler::setEventManager(EventManager *eventManager)
{
    m_eventManager = eventManager;
}

void CommandHandler::processJavaScriptCommand(QPointer<IpcClient> client, QStringList args)
{
    QString type = args.value(0);
    QString value = args.value(1);

    QEventLoop loop;

    // Process JavaScript response from Web View and tells the even loop to exit
    auto processJavaScriptResponse = [client, &loop](const QVariant &out) mutable {
        if (!client.isNull())
            client->write(out.toByteArray() + "\n");
        loop.quit();
    };

    if (type == "string") {
        m_eventManager->webView()->page()->runJavaScript(value, processJavaScriptResponse);
    } else if (type == "file") {
        QFile file(value);
        file.open(QFile::ReadOnly);

        m_eventManager->webView()->page()->runJavaScript(file.readAll(), processJavaScriptResponse);
    } else {
        return;
    }

    // Wait for JavaScript response from the Web View
    loop.exec();
}
