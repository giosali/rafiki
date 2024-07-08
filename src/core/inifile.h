#ifndef INIFILE_H
#define INIFILE_H

#include <QString>
#include <QStringList>
#include <map>
#include <string>

class IniFile {
 public:
  IniFile() = default;
  explicit IniFile(const std::string& path);

  void BeginSection(const QString& section);
  bool Contains(const QString& key) const;
  void EndSection();
  QString GetValue(const QString& key,
                   const QString& fallback = QString{}) const;
  QStringList Keys() const;
  QStringList Sections() const;
  void SetValue(const QString& key_name, const QString& value);
  void Sync() const;

 private:
  static const QString kDefaultSection;

  QString Key(const QString& section_name, const QString& key_name) const;
  void Parse();
  QString SectionName() const;

  std::string path_;
  std::map<QString, QString> properties_;
  QString section_;
};

#endif  // INIFILE_H
