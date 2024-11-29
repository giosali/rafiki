#include "filesystementrymodel.h"

#include <memory>

#include "../bridges/filesystementrybridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

FileSystemEntryModel::FileSystemEntryModel()
    : FeatureModel{std::make_unique<FileSystemEntryBridge>()} {
  SetAltDescription(tr("Open containing folder"));
  SetCommand(kCommand);
  SetDescription(tr("Search for files on your PC and open"));
  SetIcon(Paths::GetPath(Paths::Image::kFileSystemEntry));
  SetId(17);
  SetTitle(tr("Open file"));
}

void FileSystemEntryModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool FileSystemEntryModel::ReceivesInput() const { return true; }

const QString FileSystemEntryModel::kCommand{"'"};
