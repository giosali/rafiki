#ifndef INIFILE_H
#define INIFILE_H

#include <map>
#include <string>

class IniFile {
 public:
  explicit IniFile(const std::string& file_path);

  void BeginSection(const std::string& section);
  void EndSection();
  std::string GetValue(const std::string& key) const;

 private:
  void Parse(const std::string& file_path);

  std::map<std::string, std::string> properties_;
  std::string section_;
};

#endif  // INIFILE_H
