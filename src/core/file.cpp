#include "file.h"

#include <QFile>
#include <QIODeviceBase>
#include <QJsonDocument>
#include <fstream>
#include <system_error>

QJsonDocument File::Read(Paths::Json filename) {
  auto file = QFile{Paths::GetPath(filename)};
  if (!file.exists()) {
    return {};
  }

  file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
  auto document = QJsonDocument::fromJson(file.readAll());
  return document;
}

void File::Write(const QString& path, const QJsonArray& array) {
  auto document = QJsonDocument{};
  document.setArray(array);

  auto file = QFile{path};
  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Compact));
}

void File::Write(const std::filesystem::path& path, const std::string& buffer) {
  // Ensures parent directories are created beforehand.
  auto ec = std::error_code{};
  std::filesystem::create_directories(path.parent_path(), ec);
  if (ec) {
    return;
  }

  if (auto stream = std::ofstream{path}) {
    stream << buffer;
  }
}
