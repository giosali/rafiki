#pragma once

#include <QLocalServer>
#include <QObject>
#include <QStringList>

class Server : public QObject {
  Q_OBJECT

 public:
  static QString kServerName;

  explicit Server(QObject* parent = nullptr);

  bool Listen();

 public slots:
  void ReadMessage();
  void Remove();

 signals:
  void MessageReceived(const QStringList& arguments, bool from_server);

 private:
  QLocalServer server_;
};
