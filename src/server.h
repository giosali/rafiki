#ifndef SERVER_H
#define SERVER_H

#include <QLocalServer>
#include <QObject>
#include <QString>

class Server : public QObject {
  Q_OBJECT

 public:
  static QString kServerName;

  explicit Server(QObject* parent = nullptr);

  bool Listen();

 public slots:
  void Remove();
  void SendTimestamp();

 private:
  qint64 GetTimestamp() const;

  QLocalServer server_;
};

#endif  // SERVER_H
