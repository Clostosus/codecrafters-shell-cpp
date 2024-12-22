// ReSharper disable All
#include "CommandReader.h"

#include <iostream>
#include <sstream>

#define END '\0'
#define SPACE ' '
#define SINGLE '\''
#define DOUBLE '"'
#define BACKSLASH '\\'

CommandReader::CommandReader() {
    currentState = ParserState::OutsideArgument;
    escapedNextChar = false;
};

void CommandReader::readOneLine(std::string &cmdName, std::vector<std::string> &arguments) {
    arguments.clear();
    std::string input;
    std::getline(std::cin, input);
    cmdName = input.substr(0, input.find(' ')); // Command-Name extrahieren

    // Argumente extrahieren
    std::string arg;

        std::string::iterator pos = input.begin(); // argStart = input.begin(),argEnd = input.end();
        while (*pos != SINGLE && *pos != ' ') { pos++; }
        pos++; // Leerzeichen nach cmdName ignorieren
        bool insideSingleQuotes = false, insideWord = false;
        for (pos ; pos <= input.end()+1; pos++) {
            char c = *pos, nextChar = *(pos+1);
            this->handleStateTransition(c,nextChar,arg);
            if (currentState == ParserState::OutsideArgument && !arg.empty()) {
                arguments.push_back(arg);
                arg.clear();
            }
        }
}

void CommandReader::handleStateTransition(char currentChar, char nextChar, std::string &currentArgument) {
    if(currentState != ParserState::InsideSingleQuotes) {
        if(escapedNextChar) {
            escapedNextChar = false; currentArgument.push_back(currentChar);
            return;
        }
    }

    switch (currentState) {
      case ParserState::InsideSingleQuotes:
          if(currentChar == SINGLE) {
              currentState = ParserState::OutsideArgument;
          } else {
              currentArgument.push_back(currentChar);
          }
          break;
        case ParserState::InsideWord:
            if (currentChar == ' ') {
                if(nextChar != '\\') currentState = ParserState::OutsideArgument;// Argument finished
            } else if (currentChar == SINGLE) {
                // Wechsel in Single Quotes innerhalb eines Arguments
                currentState = ParserState::InsideSingleQuotes;
            } else if(currentChar == '\0') {
                currentState = ParserState::OutsideArgument;
            }else if (currentChar == '\\') {
                if(nextChar == '\\' || nextChar == '$' || nextChar == SINGLE || nextChar == DOUBLE) {escapedNextChar = true;}
                else{currentArgument.push_back(currentChar);}
            } else {
                currentArgument.push_back(currentChar);
            }
            break;
        case ParserState::OutsideArgument:
            if (currentChar == SINGLE) {
                currentState = ParserState::InsideSingleQuotes;
            } else if (currentChar == '\0') {

            } else if (currentChar == DOUBLE) {
                currentState = ParserState::InsideDoubleQuotes;
            } else if(currentChar == '\\') {
                if(nextChar == '\\' || nextChar == '$' || nextChar == SINGLE || nextChar == DOUBLE) {escapedNextChar = true;}
                else{currentArgument.push_back(currentChar);}
            }else if(currentChar != ' ' && currentChar!= '\\') {
                currentArgument.push_back(currentChar);
                currentState = ParserState::InsideWord;
            }
            break;
        case ParserState::InsideDoubleQuotes:
            if(currentChar == BACKSLASH) {
                if(nextChar == BACKSLASH || nextChar == '$' || nextChar == DOUBLE) {escapedNextChar = true;}
                else{currentArgument.push_back(currentChar);}
            }
            else if(currentChar == DOUBLE) { currentState = ParserState::OutsideArgument; }
            else { currentArgument.push_back(currentChar); }
            break;
        default:
            throw std::runtime_error("Unknown Parser-State");
    }
}
