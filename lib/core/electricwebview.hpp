#ifndef ELECTRICWEBVIEW_HPP
#define ELECTRICWEBVIEW_HPP

#include <QString>

class QWebEngineView;

class EventManager;
class CommandHandler;
class InputEventFilter;

class ElectricWebView
{
public:
    static ElectricWebView *instance();

    void initialize();

    void runScript(const QString &transport, const QString &fileName);

    QWebEngineView *webView() const;
    void setWebView(QWebEngineView *webView);

    EventManager *eventManager() const;
    CommandHandler *commandHandler() const;
    InputEventFilter *inputEventFilter() const;

private:
    ElectricWebView();

    QWebEngineView *m_webView;
    EventManager *m_eventManager;
    CommandHandler *m_commandHandler;
    InputEventFilter *m_inputEventFilter;
};

#endif // ELECTRICWEBVIEW_HPP
