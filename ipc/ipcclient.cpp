#include "ipcclient.hpp"

IpcClient::IpcClient(QObject *parent)
    : QObject(parent)
{
}

void IpcClient::initialize()
{
}

IpcClient *IpcClient::newClient(const QStringList &args)
{
    Q_UNUSED(args);
    return NULL;
}
