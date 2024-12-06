#pragma once

#include <QObject>
#include <filesystem>

class TestIndexer : public QObject {
  Q_OBJECT

 private:
  std::filesystem::path GetParentPath() const;

 private slots:
  void Clear();
  void DeleteModel();
  void GetIds_InputfulModel();
  void GetIds_InputfulModel_data();
  void GetIds_InputlessModel();
  void GetIds_InputlessModel_data();
  void UpdateTrie();
  void UpdateTrie_data();
};
