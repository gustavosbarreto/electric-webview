#include "commandhandler.hpp"

#include <QWebEngineView>
#include <QFile>
#include <QEventLoop>
#include <QBuffer>

#include "ipcclient.hpp"
#include "eventmanager.hpp"

CommandHandler::CommandHandler(QObject *parent)
    : QObject(parent)
{
}

void CommandHandler::processCommand(const Command &command) const
{
    if (command.name() == "load") {
        if (command.arguments().isEmpty())
            m_eventManager->webView()->load(QUrl("about:blank"));
        else
            m_eventManager->webView()->load(QUrl(command.arguments().first()));
    } else if (command.name() == "stop") {
        m_eventManager->webView()->stop();
    } else if (command.name() == "reload") {
        m_eventManager->webView()->reload();
    } else if (command.name() == "back") {
        m_eventManager->webView()->back();
    } else if (command.name() == "forward") {
        m_eventManager->webView()->forward();
    } else if (command.name() == "open") {
        QString mode = command.arguments().value(0);

        if (mode == "maximized")
            m_eventManager->webView()->showMaximized();
        else if (mode == "fullscreen")
            m_eventManager->webView()->showFullScreen();
    } else if (command.name() == "close") {
        m_eventManager->webView()->close();
    } else if (command.name() == "current_url") {
        command.sendResponse(m_eventManager->webView()->url().toString().toLocal8Bit());
    } else if (command.name() == "set_html") {
        QString type = command.arguments().value(0);
        QString value = command.arguments().value(1);

        if (type == "string") {
            m_eventManager->webView()->page()->setHtml(value.toLocal8Bit());
        } else if (type == "file") {
            QFile file(value);
            file.open(QFile::ReadOnly);

            m_eventManager->webView()->page()->setHtml(file.readAll());
        }
    } else if (command.name() == "current_title") {
        command.sendResponse(m_eventManager->webView()->title().toLocal8Bit());
    } else if (command.name() == "screenshot") {
        processScreenshotCommand(command);
    } else if (command.name() == "subscribe") {
        QString eventName = command.arguments().value(0);
        QStringList events = QStringList()
                << "title_changed"
                << "url_changed"
                << "load_started"
                << "load_finished";

        if (events.contains(eventName)) {
            Event event;
            event.setName(eventName);
            event.setSubscriptionCommand(command);

            m_eventManager->subscribe(event);
        }
    } else if (command.name() == "js") {
        processJavaScriptCommand(command);
    }
}

void CommandHandler::setEventManager(EventManager *eventManager)
{
    m_eventManager = eventManager;
}

void CommandHandler::processScreenshotCommand(const Command &command) const
{
    QStringList region = command.arguments().value(0).split(',');

    int x = region.value(0).toInt();
    int y = region.value(1).toInt();
    int width = region.value(2).toInt();
    int height = region.value(3).toInt();

    QRect rect = QRect(QPoint(x, y), QSize(width, height));

    if (rect.isNull() || !rect.isValid())
        rect = QRect(QPoint(0, 0), QSize(-1, -1));

    QByteArray data;
    QBuffer buffer(&data);

    QPixmap pixmap = m_eventManager->webView()->grab(rect);
    pixmap.save(&buffer, "JPG");

    command.sendResponse(data.toBase64());
}


void CommandHandler::processJavaScriptCommand(const Command &command) const
{
    QString type = command.arguments().value(0);
    QString value = command.arguments().value(1);

    QEventLoop loop;

    // Process JavaScript response from Web View and tells the even loop to exit
    auto processJavaScriptResponse = [&command, &loop](const QVariant &out) mutable {
        if (!command.client().isNull()) {
            command.sendResponse(out.toByteArray());

            if (command.isSingleShot())
                command.client()->close();
        }
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
