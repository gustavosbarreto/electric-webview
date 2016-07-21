#include "electricwebview.hpp"

#include <QCoreApplication>
#include <QProcess>
#include <QProcessEnvironment>
#include <QTemporaryFile>
#include <QTextStream>

#include "commandhandler.hpp"
#include "eventmanager.hpp"
#include "inputeventfilter.hpp"

ElectricWebView *ElectricWebView::instance()
{
    static ElectricWebView *instance = new ElectricWebView;
    return instance;
}

void ElectricWebView::initialize()
{
    m_eventManager->bind();

    qApp->installEventFilter(m_inputEventFilter);
}

void ElectricWebView::runScript(const QString &transport, const QString &fileName)
{
    QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
    env.insert("INSTANT_WEBVIEW_TRANSPORT", transport);

    QFile aliasFile(":/scripts/shellscript.sh");
    aliasFile.open(QFile::ReadOnly);

    QFile scriptFile(fileName);
    scriptFile.open(QFile::ReadOnly);

    QTemporaryFile *file = new QTemporaryFile();
    file->open();
    file->write(aliasFile.readAll() + "\n" + scriptFile.readAll());
    file->close();

    QProcess *process = new QProcess();

    QObject::connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        file->remove();
        file->deleteLater();
        process->deleteLater();
    });

    QObject::connect(process, &QProcess::readyReadStandardOutput, [=]() {
       QTextStream(stdout) << process->readAllStandardOutput();
    });

    QObject::connect(process, &QProcess::readyReadStandardError, [=]() {
       QTextStream(stderr) << process->readAllStandardError();
    });

    process->setProcessEnvironment(env);
    process->start("sh", QStringList() << file->fileName());
}

ElectricWebView::ElectricWebView()
{
    m_eventManager = new EventManager;
    m_commandHandler = new CommandHandler();
    m_inputEventFilter = new InputEventFilter;
}

InputEventFilter *ElectricWebView::inputEventFilter() const
{
    return m_inputEventFilter;
}

CommandHandler *ElectricWebView::commandHandler() const
{
    return m_commandHandler;
}


EventManager *ElectricWebView::eventManager() const
{
    return m_eventManager;
}

QWebEngineView *ElectricWebView::webView() const
{
    return m_webView;
}

void ElectricWebView::setWebView(QWebEngineView *webView)
{
    m_webView = webView;
}
