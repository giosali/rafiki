#include "miscellaneoustab.h"

#include "../core/indexer.h"
#include "../core/models/refreshmodel.h"
#include "../core/models/trashmodel.h"
#include "./ui_miscellaneoustab.h"

MiscellaneousTab::MiscellaneousTab(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::MiscellaneousTab>()} {
  ui_->setupUi(this);

  if (auto models = Indexer::GetInstance().GetModels<RefreshModel>();
      !models.empty()) {
    ui_->refresh_check_box->setChecked(models[0]->GetIsEnabled());
  }

  if (auto models = Indexer::GetInstance().GetModels<TrashModel>();
      !models.empty()) {
    ui_->trash_check_box->setChecked(models[0]->GetIsEnabled());
  }

  connect(ui_->refresh_check_box, &QCheckBox::checkStateChanged, this,
          &MiscellaneousTab::ToggleRefreshModel);
  connect(ui_->trash_check_box, &QCheckBox::checkStateChanged, this,
          &MiscellaneousTab::ToggleTrashModel);
}

MiscellaneousTab::~MiscellaneousTab() {}

void MiscellaneousTab::ToggleRefreshModel(Qt::CheckState state) {
  if (auto models = Indexer::GetInstance().GetModels<RefreshModel>();
      !models.empty()) {
    emit ModelToggled(state, models[0]->GetId());
  }
}

void MiscellaneousTab::ToggleTrashModel(Qt::CheckState state) {
  if (auto models = Indexer::GetInstance().GetModels<TrashModel>();
      !models.empty()) {
    emit ModelToggled(state, models[0]->GetId());
  }
}
