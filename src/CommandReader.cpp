// ReSharper disable All
#include "CommandReader.h"

#include <iostream>
#include <sstream>

CommandReader::CommandReader() = default;

void CommandReader::readOneLine(std::string &cmdName, std::vector<std::string> &arguments) {
    arguments.clear();
    std::string input;
    std::getline(std::cin, input);
    // Command-Name extrahieren
    cmdName = input.substr(0, input.find(' '));

    // Argumente extrahieren
    std::string arg;

    if(input.contains('\'')) {// Process args if one arg contains '
        std::string::iterator pos = input.begin(), argStart = input.begin(),argEnd = input.end();
        while (*pos != '\'' && *pos != ' ') { pos++; }
        pos++; // Leerzeichen nach cmdName ignorieren
        bool insideSingleQuotes = false, insideWord = false;
        for (pos ; pos <= input.end(); pos++) {
            char c = *pos;
            if(c == '\'') {
                if(!insideSingleQuotes) {
                    argStart = pos; insideSingleQuotes = true;
                } else {
                    argEnd = pos; insideSingleQuotes = false;
                    arguments.push_back(arg);
                    arg.clear();
                }
            }else if(c == ' ') {
                if(!insideSingleQuotes) {
                    if(!insideWord) {
                        argStart = pos; insideWord = true;
                    }else {
                        argEnd = pos; insideWord = false;
                        arguments.push_back(arg);
                        arg.clear();
                    }
                } else {
                    arg.push_back(c);
                }
            } else {
                arg.push_back(c);
            }
        }
    }else {
        std::stringstream inputStream(input.substr(input.find(' ') + 1));// +1 um führendes ' ' zu ignorieren
        while(getline(inputStream, arg,' ')) {
            arguments.push_back(arg);
        }
    }
}

