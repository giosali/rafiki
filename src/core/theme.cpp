#include "theme.h"

#include <QFile>
#include <QIODeviceBase>
#include <QJsonDocument>
#include <QJsonObject>

Theme& Theme::GetInstance() {
  static auto instance = Theme{};
  return instance;
}

int Theme::GetBorderRadius() const { return border_radius_; }

int Theme::GetFontSize() const { return font_size_; }

bool Theme::GetIsLightMode() const { return is_light_mode_; }

QColor Theme::GetSelectionColor() const { return selection_color_; }

QColor Theme::GetViewForegroundColor() const { return view_foreground_color_; }

QColor Theme::GetWindowBackgroundColor() const {
  return window_background_color_;
}

QColor Theme::GetWindowForegroundColor() const {
  return window_foreground_color_;
}

void Theme::LoadFile(const QString& filename) {
  auto file = QFile{":/data/" + filename};
  if (!file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
    return;
  }

  auto document = QJsonDocument::fromJson(file.readAll());
  if (!document.isObject()) {
    return;
  }

  auto object = document.object();

  if (auto key = "borderRadius"; object.contains(key)) {
    border_radius_ = object[key].toInt();
  }

  if (auto key = "fontSize"; object.contains(key)) {
    font_size_ = object[key].toInt();
  }

  if (auto key = "isLightMode"; object.contains(key)) {
    is_light_mode_ = object[key].toBool();
  }

  if (auto key = "selectionColor"; object.contains(key)) {
    selection_color_ = object[key].toString();
  }

  if (auto key = "viewForegroundColor"; object.contains(key)) {
    view_foreground_color_ = object[key].toString();
  }

  if (auto key = "windowBackgroundColor"; object.contains(key)) {
    window_background_color_ = object[key].toString();
  }

  if (auto key = "windowForegroundColor"; object.contains(key)) {
    window_foreground_color_ = object[key].toString();
  }

  emit Changed(this);
}
