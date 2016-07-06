#include "commandhandler.hpp"

#include <QWebEngineView>
#include <QFile>
#include <QEventLoop>
#include <QBuffer>
#include <QProcess>
#include <QDesktopWidget>
#include <QApplication>

#include "eventmanager.hpp"
#include "instantwebview.hpp"
#include "inputeventfilter.hpp"

#include <ipc/ipcclient.hpp>

CommandHandler::CommandHandler(QObject *parent)
    : QObject(parent)
{
}

void CommandHandler::processCommand(const Command &command) const
{
    QWebEngineView *webView = InstantWebView::instance()->webView();

    if (command.name() == "load") {
        if (command.arguments().isEmpty())
            webView->load(QUrl("about:blank"));
        else
            webView->load(QUrl(command.arguments().first()));
    } else if (command.name() == "stop") {
        webView->stop();
    } else if (command.name() == "reload") {
        webView->reload();
    } else if (command.name() == "back") {
        webView->back();
    } else if (command.name() == "forward") {
        webView->forward();
    } else if (command.name() == "open") {
        QString mode = command.arguments().value(0);

        if (mode == "maximized") {
            webView->showMaximized();
        } else if (mode == "fullscreen") {
            webView->setGeometry(qApp->desktop()->screenGeometry());
            webView->showFullScreen();
        }
    } else if (command.name() == "close") {
        webView->close();
    } else if (command.name() == "current_url") {
        command.sendResponse(webView->url().toString().toLocal8Bit());
    } else if (command.name() == "set_html") {
        QString type = command.arguments().value(0);
        QString value = command.arguments().mid(1, -1).join(' ');

        if (type == "string") {
            webView->page()->setHtml(value.toLocal8Bit());
        } else if (type == "file") {
            QFile file(value);
            file.open(QFile::ReadOnly);

            webView->page()->setHtml(file.readAll());
        }
    } else if (command.name() == "get_html") {
        QString format = command.arguments().value(0);

        QEventLoop loop;

        if (format == "html") {
            webView->page()->toHtml([&command, &loop](const QString &html) {
                if (!command.client().isNull()) {
                    command.sendResponse(QUrl::toPercentEncoding(html));

                    if (command.isSingleShot())
                        command.client()->close();
                }
                loop.quit();
            });
        } else if (format == "text") {
            webView->page()->toPlainText([&command, &loop](const QString &text) {
                if (!command.client().isNull()) {
                    command.sendResponse(QUrl::toPercentEncoding(text));

                    if (command.isSingleShot())
                        command.client()->close();
                }
                loop.quit();
            });
        } else {
            return;
        }

        loop.exec();
    } else if (command.name() == "current_title") {
        command.sendResponse(webView->title().toLocal8Bit());
    } else if (command.name() == "screenshot") {
        processScreenshotCommand(command);
    } else if (command.name() == "subscribe") {
        QString eventName = command.arguments().value(0);
        QStringList events = QStringList()
                << "title_changed"
                << "url_changed"
                << "load_started"
                << "load_finished"
                << "user_activity"
                << "info_message_raised"
                << "warning_message_raised"
                << "error_message_raised";

        if (events.contains(eventName)) {
            Event event(command);
            event.setName(eventName);

            InstantWebView::instance()->eventManager()->subscribe(event);
        }
    } else if (command.name() == "exec_js") {
        processJavaScriptCommand(command);
    } else if (command.name() == "idle_time") {
        command.sendResponse(QString("%1").arg(InstantWebView::instance()->inputEventFilter()->idle()).toLocal8Bit());
    } else if (command.name() == "block_user_activity") {
        bool block = QVariant(command.arguments().value(0)).toBool();
        InstantWebView::instance()->inputEventFilter()->setBlock(block);
    } else if (command.name() == "exec_cmd") {
        bool sync = command.arguments().value(0) == "sync";

        if (sync) {
            QProcess *process = new QProcess;
            process->start(command.arguments().mid(1, -1).join(' '));
            process->waitForFinished(-1);
            command.sendResponse(QUrl::toPercentEncoding(process->readAllStandardOutput()));
        } else {
            QProcess::startDetached(command.arguments().mid(1, -1).join(' '));
        }
    }
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

    QPixmap pixmap = InstantWebView::instance()->webView()->grab(rect);
    pixmap.save(&buffer, "JPG");

    command.sendResponse(data.toBase64());
}


void CommandHandler::processJavaScriptCommand(const Command &command) const
{
    QString type = command.arguments().value(0);
    QString value = command.arguments().mid(1, -1).join(' ');

    QEventLoop loop;

    // Process JavaScript response from Web View and tells the even loop to exit
    auto processJavaScriptResponse = [&command, &loop](const QVariant &out) mutable {
        if (!command.client().isNull()) {
            command.sendResponse(QUrl::toPercentEncoding(out.toString()));

            if (command.isSingleShot())
                command.client()->close();
        }
        loop.quit();
    };

    if (type == "string") {
        InstantWebView::instance()->webView()->page()->runJavaScript(value, processJavaScriptResponse);
    } else if (type == "file") {
        QFile file(value);
        file.open(QFile::ReadOnly);

        InstantWebView::instance()->webView()->page()->runJavaScript(file.readAll(), processJavaScriptResponse);
    } else {
        return;
    }

    // Wait for JavaScript response from the Web View
    loop.exec();
}
