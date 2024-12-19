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

    //if(input.contains('\'')) {// Process args if one arg contains '
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
                        if(pos != input.end() && *(pos+1).base() != ' ') {
                            argStart = pos+1; insideWord = true;
                        } else { continue; }
                    }else {
                        argEnd = pos; insideWord = false;
                        std::cout << arg << std::endl;
                        arguments.push_back(arg);
                        arg.clear();
                    }
                } else {
                    arg.push_back(c);
                }
            } else {
                if(c != '\0') arg.push_back(c);
            }
        }
    //}
}

