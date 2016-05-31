#include "ipcclient.hpp"

IpcClient::IpcClient(QObject *parent)
    : QObject(parent)
{
}

void IpcClient::initialize()
{
}

IpcClient *IpcClient::newClient(const QStringList &args, bool reverse)
{
    Q_UNUSED(args);
    Q_UNUSED(reverse);
    return NULL;
}
