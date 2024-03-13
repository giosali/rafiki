#include "client.h"

#include <QIODeviceBase>

#include "server.h"

Client::Client(QObject* parent) : QObject(parent), socket_(this) {
  out_.setDevice(&socket_);

  QObject::connect(&socket_, &QLocalSocket::errorOccurred, this,
                   &Client::ProcessError);
}

void Client::Connect(const QString& message) {
  socket_.abort();
  socket_.connectToServer(Server::kServerName, QIODeviceBase::WriteOnly);
  if (!socket_.waitForConnected(1000)) {
    return;
  }

  auto block = QByteArray();
  auto out = QDataStream(&block, QIODeviceBase::WriteOnly);
  out << message;
  out.device()->seek(0);
  socket_.write(block);
  socket_.flush();
}

void Client::ProcessError(QLocalSocket::LocalSocketError socket_error) {
  switch (socket_error) {
    // Encountered after a crash/forcequit.
    case QLocalSocket::ConnectionRefusedError:
      emit ConnectionRefused();
      break;
    // Encountered when everything is running normally and the server simply
    // responds as intended.
    case QLocalSocket::PeerClosedError:
      break;
    // Encountered when the server has been removed and a connection to it is
    // attempted. This should theoretically never be reached.
    case QLocalSocket::ServerNotFoundError:
      break;
  }
}
