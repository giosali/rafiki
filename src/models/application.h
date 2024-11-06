#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyCombination>
#include <QSettings>
#include <QString>
#include <filesystem>

#include "result.h"

class Application : public Result {
  Q_OBJECT

 public:
  explicit Application(const std::filesystem::path& desktop_entry_path,
                       QSettings& desktop_entry_file, const QString& icon);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  QString RemoveFieldCodes(const std::string& exec) const;

  QString exec_;
};

#endif  // APPLICATION_H
