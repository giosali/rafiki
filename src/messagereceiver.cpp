#include "messagereceiver.h"
#include "argumentparser.h"

MessageReceiver::MessageReceiver(QObject* parent)
    : QObject(parent)
{
}

void MessageReceiver::receivedMessage(const int instanceId, const QByteArray &message)
{
    ArgumentParser argumentParser;
    argumentParser.parse(message);
}
