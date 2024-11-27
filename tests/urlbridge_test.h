#pragma once

#include <QObject>
#include <QTest>
#include <memory>

#include "core/bridges/urlbridge.h"
#include "core/models/urlmodel.h"

class TestUrlBridge : public QObject {
  Q_OBJECT

 public:
  TestUrlBridge();

 private:
  UrlBridge bridge_{};
  std::unique_ptr<UrlModel> model_{nullptr};

 private slots:
  void ProcessInput_Invalid();
  void ProcessInput_Invalid_data();
  void ProcessInput_Valid();
  void ProcessInput_Valid_data();
};
