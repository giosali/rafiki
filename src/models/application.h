#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyCombination>
#include <QSettings>
#include <QString>
#include <filesystem>
#include <unordered_set>

#include "result.h"

class Application : public Result {
  Q_OBJECT

 public:
  explicit Application(const std::filesystem::path& desktop_entry_path,
                       QSettings& desktop_entry_file);

  std::unordered_set<std::string> Tokenize() const override;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  QString exec_;
};

#endif  // APPLICATION_H
