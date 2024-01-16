#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QByteArray>

class CommandLine {
 public:
  CommandLine() = default;

  void Parse(const QByteArray &line) const;

 private:
  void HandleToggle() const;
};

#endif  // COMMANDLINE_H
