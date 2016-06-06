#include "inputeventfilter.hpp"

#include <QEvent>

InputEventFilter::InputEventFilter(QObject *parent)
    : QObject(parent)
    , m_block(false)
{
    m_idleTime.start();
}

int InputEventFilter::idle() const
{
    return m_idleTime.elapsed();
}

bool InputEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress
        || event->type() == QEvent::MouseMove
        || event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::MouseButtonDblClick) {
        if (m_block)
            return true;

        emit activity(m_idleTime.elapsed());
        m_idleTime.restart();
    }

    return QObject::eventFilter(obj, event);
}

bool InputEventFilter::block() const
{
    return m_block;
}

void InputEventFilter::setBlock(bool block)
{
    m_block = block;
}
