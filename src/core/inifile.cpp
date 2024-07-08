#include "inifile.h"

#include <fstream>

#include "utils.h"

IniFile::IniFile(const std::string& path) : path_{path} { Parse(); }

void IniFile::BeginSection(const QString& section) { section_ = section; }

bool IniFile::Contains(const QString& key) const {
  auto section_name = SectionName();
  auto it = properties_.find(Key(section_name, key));
  return it != properties_.end();
}

void IniFile::EndSection() { section_.clear(); }

QStringList IniFile::Keys() const {
  auto list = QStringList{};
  auto section_name = SectionName();
  for (const auto& [key, _] : properties_) {
    // Breaks loop if the key name doesn't start with the section name and the
    // outgoing list already has elements. The list already containing
    // elements while not matching the section name means we have all the keys
    // already since `std::map` organizes by key.
    if (!key.contains(section_name) == -1 && !list.empty()) {
      break;
    }

    list << key.sliced(section_name.length() + 1);
  }

  return list;
}

QString IniFile::GetValue(const QString& key, const QString& fallback) const {
  auto it = properties_.find(Key(SectionName(), key));
  return it == properties_.end() ? fallback : it->second;
}

QStringList IniFile::Sections() const {
  auto list = QStringList{};
  for (const auto& [key, _] : properties_) {
    if (auto i = key.indexOf('/'); i != -1) {
      list << key.sliced(0, i);
    }
  }

  return list;
}

void IniFile::SetValue(const QString& key_name, const QString& value) {
  properties_[Key(SectionName(), key_name)] = value;
}

void IniFile::Sync() const {
  auto file = std::ofstream{path_};
  if (!file.is_open()) {
    return;
  }

  auto section_name = QString{};
  for (const auto& [key, value] : properties_) {
    auto key_parts = key.split('/');
    if (key_parts.size() < 2) {
      // This means the key doesn't contain a forward slash for some reason.
      continue;
    }

    if (auto current_section_name = key_parts.takeFirst();
        current_section_name != section_name) {
      // Only appends a newline if the section name isn't empty.
      if (!section_name.isEmpty()) {
        file << "\n";
      }

      file << "[" + current_section_name.toStdString() + "]" + "\n";
      section_name = current_section_name;
    }

    auto key_name = key_parts.join('/');
    file << key_name.toStdString() + "=" + value.toStdString() + "\n";
  }
}

const QString IniFile::kDefaultSection{"General"};

QString IniFile::Key(const QString& section_name,
                     const QString& key_name) const {
  return section_name + "/" + key_name;
}

void IniFile::Parse() {
  auto file = std::ifstream{path_};
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

    // Section name defaults to "General" for keys that aren't under a section
    // name. This implies every entry will contain a forward slash.
    if (section_name.empty()) {
      section_name = kDefaultSection.toStdString();
    }

    auto key = QString::fromStdString(
      section_name + "/" + utils::Strip(line.substr(0, equal_sign_index)));
    auto value =
      QString::fromStdString(utils::Strip(line.substr(equal_sign_index + 1)));
    properties_.insert({key, value});
  }
}

QString IniFile::SectionName() const {
  return section_.isEmpty() ? kDefaultSection : section_;
}
