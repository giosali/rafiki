#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QLocalSocket>
#include <QObject>

class Client : public QObject {
  Q_OBJECT

 public:
  explicit Client(QObject* parent = nullptr);

  void Connect();

 public slots:
  void ProcessError(QLocalSocket::LocalSocketError socket_error);
  void ReadTimestamp();

 signals:
  void ConnectionRefused();

 private:
  QDataStream in_;
  QLocalSocket socket_;
};

#endif  // CLIENT_H
