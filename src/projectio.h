#ifndef PROJECTIO_H
#define PROJECTIO_H

#include <QString>
#include <memory>
#include <unordered_map>
#include <vector>

#include "autocompletemap.h"
#include "datamodel.h"

class ProjectIO {
 public:
  enum class DataFile { kWebSearches };
  enum class ImageFile { kQuestionMark };

  explicit ProjectIO();

  static QString GetDataFilePath(DataFile file);
  static QString GetImageFilePath(ImageFile file);
  std::vector<std::unique_ptr<DataModel>> FindDataModels(const QString& input);

 private:
  template <typename T>
  void ParseDataFile(const QString& path);

  AutocompleteMap autocomplete_map_;
  std::unordered_map<QString, std::vector<std::unique_ptr<DataModel>>>
      data_model_map_;
};

#endif  // PROJECTIO_H
