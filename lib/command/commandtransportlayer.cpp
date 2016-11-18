#include "commandtransportlayer.hpp"

CommandTransportLayer::CommandTransportLayer(QObject *parent)
    : QObject(parent)
{
}

void CommandTransportLayer::setOptions(const QStringList &options)
{
    m_options = options;
}
