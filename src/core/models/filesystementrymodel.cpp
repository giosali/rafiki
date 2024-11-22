#include "filesystementrymodel.h"

#include <memory>

#include "../bridges/filesystementrybridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

FileSystemEntryModel::FileSystemEntryModel()
    : FeatureModel{std::make_unique<FileSystemEntryBridge>()} {
  SetAltDescription("Show in folder");
  SetCommand("'");
  SetDescription("Search for files on your PC and open");
  SetIcon(Paths::GetPath(Paths::Image::kFileSystemEntry));
  SetId(17);
  SetTitle("Open file");
}

void FileSystemEntryModel::Accept(FeatureVisitor& v) { v.Visit(this); }

QString FileSystemEntryModel::FormatCommand() const { return GetCommand(); }

bool FileSystemEntryModel::ReceivesInput() const { return true; }
