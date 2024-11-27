#pragma once

#include <QJsonObject>
#include <QObject>
#include <QTest>

class TestWebSearchBridge : public QObject {
  Q_OBJECT

 public:
  TestWebSearchBridge();

 private:
  QJsonObject object_with_formattable_title_{};
  QJsonObject object_with_unformattable_title_{};

 private slots:
  void ProcessInput_FormattableTitle();
  void ProcessInput_FormattableTitle_data();
  void ProcessInput_UnformattableTitle();
  void ProcessInput_UnformattableTitle_data();
};
