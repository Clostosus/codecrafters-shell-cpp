#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H
#include <vector>
#include <string>

class ArgumentsParser {
public:
    static void replaceHomedirSymbols(std::vector<std::string> &arguments);
};



#endif //ARGUMENTSPARSER_H
