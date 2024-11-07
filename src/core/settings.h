#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonDocument>
#include <cstdint>
#include <vector>

class Settings {
 public:
  static Settings& GetInstance();
  std::vector<uint64_t> GetDefaultSearchResultIds() const;
  bool GetRunOnStartup() const;
  void Update(const QJsonDocument& document);

 private:
  Settings() = default;
  Settings(const Settings&) = delete;

  Settings operator=(const Settings&) = delete;

  std::vector<uint64_t> default_search_result_ids_{1, 2, 3};
  bool run_on_startup_{true};
};

#endif  // SETTINGS_H
