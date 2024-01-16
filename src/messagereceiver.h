#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QByteArray>
#include <QObject>

class MessageReceiver : public QObject {
  Q_OBJECT

 public:
  explicit MessageReceiver(QObject *parent = nullptr);

 public slots:
  void receivedMessage(const int instanceId, const QByteArray &message);
};

#endif  // MESSAGERECEIVER_H
