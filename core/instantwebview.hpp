#ifndef INSTANTWEBVIEW_HPP
#define INSTANTWEBVIEW_HPP

class QWebEngineView;

class EventManager;
class CommandHandler;
class InputEventFilter;

class InstantWebView
{
public:
    static InstantWebView *instance();

    void initialize();

    QWebEngineView *webView() const;
    EventManager *eventManager() const;
    CommandHandler *commandHandler() const;
    InputEventFilter *inputEventFilter() const;

private:
    InstantWebView();

    QWebEngineView *m_webView;
    EventManager *m_eventManager;
    CommandHandler *m_commandHandler;
    InputEventFilter *m_inputEventFilter;
};

#endif // INSTANTWEBVIEW_HPP
