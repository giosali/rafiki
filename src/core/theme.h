#pragma once

#include <QColor>
#include <QObject>
#include <QString>

class Theme final : public QObject {
  Q_OBJECT

 public:
  static Theme& GetInstance();
  int GetBorderRadius() const;
  int GetFontSize() const;
  QColor GetSelectionColor() const;
  QColor GetWindowBackgroundColor() const;
  QColor GetWindowForegroundColor() const;
  void LoadFile(const QString& filename);

 signals:
  void Changed(Theme* theme);

 private:
  Theme() = default;
  Theme(const Theme&) = delete;

  Theme operator=(const Theme&) = delete;

  int border_radius_{};
  int font_size_{};
  QColor selection_color_{};
  QColor window_background_color_{};
  QColor window_foreground_color_{};
};
