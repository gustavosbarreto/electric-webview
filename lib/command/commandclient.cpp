#include "commandclient.hpp"

CommandClient::CommandClient(QObject *parent)
    : QObject(parent)
{
}

void CommandClient::initialize()
{
}

CommandClient *CommandClient::newClient(const QStringList &args, bool reverse)
{
    Q_UNUSED(args);
    Q_UNUSED(reverse);
    return NULL;
}
