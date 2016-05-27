#ifndef INSTANTWEBVIEW_HPP
#define INSTANTWEBVIEW_HPP

class QWebEngineView;

class EventManager;
class CommandHandler;

class InstantWebView
{
public:
    static InstantWebView *instance();

    void initialize();

    QWebEngineView *webView() const;
    EventManager *eventManager() const;
    CommandHandler *commandHandler() const;

private:
    InstantWebView();

    QWebEngineView *m_webView;
    EventManager *m_eventManager;
    CommandHandler *m_commandHandler;
};

#endif // INSTANTWEBVIEW_HPP
