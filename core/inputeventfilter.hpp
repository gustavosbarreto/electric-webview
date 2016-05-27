#ifndef INPUTEVENTFILTER_HPP
#define INPUTEVENTFILTER_HPP

#include <QObject>
#include <QTime>

class InputEventFilter: public QObject
{
    Q_OBJECT

public:
    InputEventFilter(QObject *parent = 0);

    int idle() const;

private:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    QTime m_idleTime;
};

#endif // INPUTEVENTFILTER_HPP
