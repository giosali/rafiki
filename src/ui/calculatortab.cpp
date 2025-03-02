#include "calculatortab.h"

#include "../core/indexer.h"
#include "../core/models/calculatormodel.h"
#include "./ui_calculatortab.h"

CalculatorTab::CalculatorTab(QWidget* parent)
    : ToggleTab{parent}, ui_{std::make_unique<Ui::CalculatorTab>()} {
  ui_->setupUi(this);

  // Checks or unchecks QCheckBox control.
  if (auto models = Indexer::GetInstance().GetModels<CalculatorModel>();
      !models.empty()) {
    ui_->enabled_check_box->setChecked(models[0]->GetIsEnabled());
  }

  connect(ui_->enabled_check_box, &QCheckBox::checkStateChanged, this,
          &CalculatorTab::ToggleCalculatorModel);
}

CalculatorTab::~CalculatorTab() {}

void CalculatorTab::TranslateUi() { ui_->retranslateUi(this); }

void CalculatorTab::ToggleCalculatorModel(Qt::CheckState state) {
  if (auto models = Indexer::GetInstance().GetModels<CalculatorModel>();
      !models.empty()) {
    emit ModelToggled(state, models[0]->GetId());
  }
}
