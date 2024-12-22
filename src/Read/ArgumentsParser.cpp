#include "ArgumentsParser.h"

#include "../Process/FileSearcher.h"

void ArgumentsParser::replaceHomedirSymbols(std::vector<std::string> & arguments) {
    const std::string homedir = FileSearcher::getHomedir();
    for (auto argument : arguments) {
        size_t pos = 0;
        do {
            pos = argument.find('~', pos);
            if(pos != std::string::npos) {argument = argument.replace(pos,1,homedir);} else {
                break;
            }
            pos++;
        }while( pos != std::string::npos);
    }
}

