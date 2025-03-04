#pragma once

#include <QObject>

class TestUtilities : public QObject {
  Q_OBJECT

 private slots:
  void Combine();
  void Combine_data();
  void RemoveAccents();
  void RemoveAccents_data();
  void Split_QString();
  void Split_QString_data();
  void Split_StdString();
  void Split_StdString_data();
};
