#include "server.h"

#include <QIODeviceBase>
#include <QLocalSocket>

QString Server::kServerName = QStringLiteral("rafiki");

Server::Server(QObject* parent) : QObject{parent}, server_{this} {
  QObject::connect(&server_, &QLocalServer::newConnection, this,
                   &Server::ReadMessage);
}

bool Server::Listen() { return server_.listen(kServerName); }

void Server::ReadMessage() {
  auto client_connection = server_.nextPendingConnection();

  if (client_connection->bytesAvailable() <= 0) {
    client_connection->waitForReadyRead();
  }

  QObject::connect(client_connection, &QLocalSocket::disconnected,
                   client_connection, &QLocalSocket::deleteLater);

  auto in = QDataStream{client_connection};
  auto message = QString{};
  in >> message;
  emit MessageReceived(message.split(' '), true);

  client_connection->disconnectFromServer();
}

void Server::Remove() { server_.removeServer(kServerName); }
