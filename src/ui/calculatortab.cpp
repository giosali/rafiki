#include "calculatortab.h"

#include "../core/indexer.h"
#include "../core/models/calculatormodel.h"
#include "./ui_calculatortab.h"

CalculatorTab::CalculatorTab(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::CalculatorTab>()} {
  ui_->setupUi(this);

  // Checks or unchecks QCheckBox control.
  if (auto models = Indexer::GetInstance().GetModels<CalculatorModel>();
      !models.empty()) {
    auto model = models[0];
    ui_->enabled_check_box->setChecked(model->GetIsEnabled());
  }

  connect(ui_->enabled_check_box, &QCheckBox::checkStateChanged, this,
          &CalculatorTab::ToggleCalculatorModel);
}

CalculatorTab::~CalculatorTab() {}

void CalculatorTab::ToggleCalculatorModel(Qt::CheckState state) {
  auto models = Indexer::GetInstance().GetModels<CalculatorModel>();
  if (models.empty()) {
    return;
  }

  auto model = models[0];
  switch (state) {
    case Qt::Unchecked:
      emit ModelDisabled(model);
      break;
    case Qt::Checked:
      emit ModelEnabled(model);
      break;
    default:
      break;
  }
}
