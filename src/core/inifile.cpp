#include "inifile.h"

#include <fstream>

#include "utils.h"

IniFile::IniFile(const std::string& file_path) { Parse(file_path); }

void IniFile::BeginSection(const std::string& section) { section_ = section; }

void IniFile::EndSection() { section_.clear(); }

std::string IniFile::GetValue(const std::string& key) const {
  auto it = properties_.find(section_.empty() ? key : section_ + "/" + key);
  return it == properties_.end() ? std::string{} : it->second;
}

void IniFile::Parse(const std::string& file_path) {
  auto file = std::ifstream{file_path};
  for (std::string line, section_name; std::getline(file, line);) {
    if (line.empty()) {
      continue;
    }

    line = utils::Strip(line);
    switch (line[line.find_first_not_of(' ')]) {
      case '#':
      case ';':
        continue;
      case '[':
        if (line[line.find_last_not_of(' ')] != ']') {
          break;
        }

        section_name = line.substr(1, line.length() - 2);
        continue;
    }

    auto equal_sign_index = line.find('=');
    if (equal_sign_index == std::string::npos) {
      continue;
    }

    auto key = utils::Strip(line.substr(0, equal_sign_index));
    auto value = utils::Strip(line.substr(equal_sign_index + 1));
    properties_.insert(
      {section_name.empty() ? key : section_name + "/" + key, value});
  }
}
