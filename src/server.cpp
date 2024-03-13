#include "server.h"

#include <QByteArray>
#include <QIODeviceBase>
#include <QLocalSocket>
#include <chrono>

QString Server::kServerName = QStringLiteral("rafiki");

Server::Server(QObject* parent) : QObject(parent), server_(this) {
  QObject::connect(&server_, &QLocalServer::newConnection, this,
                   &Server::SendTimestamp);
}

bool Server::Listen() { return server_.listen(kServerName); }

void Server::Remove() { server_.removeServer(kServerName); }

void Server::SendTimestamp() {
  auto block = QByteArray();
  auto out = QDataStream(&block, QIODeviceBase::WriteOnly);

  auto timestamp = GetTimestamp();
  out << timestamp;

  auto client_connection = server_.nextPendingConnection();
  QObject::connect(client_connection, &QLocalSocket::disconnected,
                   client_connection, &QLocalSocket::deleteLater);

  client_connection->write(block);
  client_connection->flush();
  client_connection->disconnectFromServer();
}

qint64 Server::GetTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time_since_epoch = now.time_since_epoch();
  auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch);
  return static_cast<qint64>(duration.count());
}
