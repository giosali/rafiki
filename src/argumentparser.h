#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QStringList>

class ArgumentParser {
public:
    ArgumentParser() = default;

    void parse(const QByteArray &commandLine);
};

#endif // ARGUMENTPARSER_H
