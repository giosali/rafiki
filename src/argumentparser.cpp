#include "argumentparser.h"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include <iostream>

void ArgumentParser::parse(const QByteArray &commandLine)
{
    QCommandLineParser parser;

    // Builtin options.
    parser.addHelpOption();
    parser.addVersionOption();

    // Custom options.
    QCommandLineOption toggleOption{"toggle", "Toggles the visibility of the main input window."};
    parser.addOption(toggleOption);

    QStringList arguments = QString(commandLine).split(" ");
    parser.process(arguments);

    if (parser.isSet(toggleOption)) {
        std::cout << "Toggle was set" << std::endl;
    }
}
