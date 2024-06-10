#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QLocalSocket>
#include <QObject>
#include <QString>

class Client : public QObject {
  Q_OBJECT

 public:
  explicit Client(QObject* parent = nullptr);

  void Connect(const QString& message);

 public slots:
  void ProcessError(QLocalSocket::LocalSocketError socket_error);

 signals:
  void ConnectionRefused();

 private:
  QDataStream out_;
  QLocalSocket socket_;
};

#endif  // CLIENT_H
