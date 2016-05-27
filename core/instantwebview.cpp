#include "instantwebview.hpp"

#include <QWebEngineView>
#include <QCoreApplication>

#include "commandhandler.hpp"
#include "eventmanager.hpp"
#include "inputeventfilter.hpp"

InstantWebView *InstantWebView::instance()
{
    static InstantWebView *instance = new InstantWebView;
    return instance;
}

void InstantWebView::initialize()
{
    m_eventManager->bind();

    qApp->installEventFilter(m_inputEventFilter);
}

InstantWebView::InstantWebView()
{
    m_webView = new QWebEngineView();
    m_eventManager = new EventManager;
    m_commandHandler = new CommandHandler();
    m_inputEventFilter = new InputEventFilter;
}

InputEventFilter *InstantWebView::inputEventFilter() const
{
    return m_inputEventFilter;
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
