#ifndef INPUT_H
#define INPUT_H

#include <QString>

class Input {
 public:
  Input() = default;
  explicit Input(const QString& text);

  QString Argument() const;
  QString Command() const;
  bool IsCommandRedundant() const;
  bool IsEmpty() const;
  QString ToString() const;

 private:
  QString argument_;
  QString command_;
  QString text_;
};

#endif  // INPUT_H
