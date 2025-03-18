#pragma once

#include <QObject>

class TestUtilities : public QObject {
  Q_OBJECT

 private slots:
  void RemoveAccents();
  void RemoveAccents_data();
};
