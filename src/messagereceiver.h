#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QByteArray>
#include <QObject>

class MessageReceiver : public QObject {
  Q_OBJECT

 public:
  explicit MessageReceiver(QObject *parent = nullptr);

 public slots:
  void ReceivedMessage(const int instance_id, const QByteArray &message) const;
};

#endif  // MESSAGERECEIVER_H
