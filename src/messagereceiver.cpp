#include "messagereceiver.h"

#include "commandline.h"
#include "mainwindow.h"

MessageReceiver::MessageReceiver(QObject *parent) : QObject(parent) {}

void MessageReceiver::ReceivedMessage(const int instance_id,
                                      const QByteArray &message) const {
  auto command_line = CommandLine();
  command_line.Parse(message);
}
