#include "messagereceiver.h"

#include "commandline.h"
#include "mainwindow.h"

MessageReceiver::MessageReceiver(QObject *parent) : QObject(parent) {}

void MessageReceiver::receivedMessage(const int instanceId,
                                      const QByteArray &message) {
  CommandLine command_line;
  command_line.Parse(message);
}
