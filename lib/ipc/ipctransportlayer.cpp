#include "ipctransportlayer.hpp"

IpcTransportLayer::IpcTransportLayer(QObject *parent)
    : QObject(parent)
{
}

void IpcTransportLayer::setOptions(const QStringList &options)
{
    m_options = options;
}
