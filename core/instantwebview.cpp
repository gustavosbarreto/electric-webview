#include "instantwebview.hpp"

#include <QWebEngineView>

#include "commandhandler.hpp"
#include "eventmanager.hpp"

InstantWebView *InstantWebView::instance()
{
    static InstantWebView *instance = new InstantWebView;
    return instance;
}

void InstantWebView::initialize()
{
    m_eventManager->bind();
}

InstantWebView::InstantWebView()
{
    m_webView = new QWebEngineView();
    m_eventManager = new EventManager;
    m_commandHandler = new CommandHandler();
}

CommandHandler *InstantWebView::commandHandler() const
{
    return m_commandHandler;
}


EventManager *InstantWebView::eventManager() const
{
    return m_eventManager;
}

QWebEngineView *InstantWebView::webView() const
{
    return m_webView;
}
