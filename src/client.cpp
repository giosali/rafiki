#include "client.h"

#include <QIODeviceBase>

#include "server.h"

Client::Client(QObject* parent) : QObject(parent), socket_(this) {
  in_.setDevice(&socket_);

  QObject::connect(&socket_, &QLocalSocket::readyRead, this,
                   &Client::ReadTimestamp);
  QObject::connect(&socket_, &QLocalSocket::errorOccurred, this,
                   &Client::ProcessError);
}

void Client::Connect() {
  socket_.abort();
  socket_.connectToServer(Server::kServerName, QIODeviceBase::ReadOnly);
}

void Client::ProcessError(QLocalSocket::LocalSocketError socket_error) {
  qDebug() << socket_error;
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

void Client::ReadTimestamp() {
  in_.startTransaction();
  qint64 timestamp;
  in_ >> timestamp;
  if (!in_.commitTransaction()) {
    return;
  }

  qDebug() << "timestamp =" << timestamp;
}
