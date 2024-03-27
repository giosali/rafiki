#ifndef INPUT_H
#define INPUT_H

#include <QString>

class Input {
 public:
  Input() = default;
  explicit Input(const QString& text);

  QString GetArg() const;
  QString GetCmd() const;
  QString GetFull() const;
  bool IsEmpty() const;

 private:
  QString arg_;
  QString cmd_;
  QString full_;
};

#endif  // INPUT_H
