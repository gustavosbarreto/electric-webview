#include "inputeventfilter.hpp"

#include <QEvent>

InputEventFilter::InputEventFilter(QObject *parent)
    : QObject(parent)
{
    m_idleTime.start();
}

int InputEventFilter::idle() const
{
    return m_idleTime.elapsed();
}

bool InputEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::MouseMove) {
        m_idleTime.restart();
    }

    return QObject::eventFilter(obj, event);
}
