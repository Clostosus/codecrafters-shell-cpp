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

void CommandReader::readOneLine(std::string &cmdName, std::vector<std::string> &arguments, CommandManager &cmdManager) {
    arguments.clear(); cmdName.clear();
    std::string input;
    readCharacterByCharacter(input, cmdManager);
    if (input.empty()) {cmdName = ""; return;}

    // Argumente extrahieren
    std::string arg;
    std::string::iterator pos = input.begin();

    for (pos ; pos <= input.end()+1; pos++) {
        char c = *pos, nextChar = *(pos+1);
        this->handleStateTransition(c,nextChar,arg);
        if (currentState == ParserState::OutsideArgument && !arg.empty()) {
             arguments.push_back(arg);
             arg.clear();
        }
    }
    cmdName = arguments.front();
    arguments.erase(arguments.begin());
}

void CommandReader::readCharacterByCharacter(std::string &currentInput, CommandManager &manager) {
    char c;
    while (true) {
        c = static_cast<char>(getchar());
        if (c == '\n') {
            return; // Input line completed
        } else if (c == '\t') {
            // Autocompletion for Tab Key
            std::vector<std::string> * suggestions = manager.getAllNamesWithPrefix(currentInput);
            if (suggestions && suggestions->size() == 1) {
                 currentInput = suggestions->front();
                 std::cout << "\r" << currentInput;
            }
            delete suggestions;
        } else {
            currentInput += c;
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
            if (currentChar == SPACE) {
                if(nextChar != '\\') currentState = ParserState::OutsideArgument;// Argument finished
            } else if (currentChar == SINGLE) {
                // Wechsel in Single Quotes innerhalb eines Arguments
                currentState = ParserState::InsideSingleQuotes;
            } else if(currentChar == '\0') {
                currentState = ParserState::OutsideArgument;
            }else if (currentChar == '\\') {
                escapedNextChar = true;
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
                if(nextChar == '\\' || nextChar == '$' || nextChar == SINGLE || nextChar == DOUBLE || nextChar == SPACE) {escapedNextChar = true;}
                else{currentArgument.push_back(currentChar);}
            }else if(currentChar != SPACE && currentChar!= '\\') {
                currentArgument.push_back(currentChar);
                currentState = ParserState::InsideWord;
            }
            break;
        case ParserState::InsideDoubleQuotes:
            if(currentChar == BACKSLASH) {
                if(nextChar == BACKSLASH || nextChar == '$' || nextChar == DOUBLE) {escapedNextChar = true;}
                else{currentArgument.push_back(currentChar);}
            }
            else if(currentChar == DOUBLE) {
                if(nextChar != SPACE){currentState = InsideWord;}else{currentState = OutsideArgument;}
            }
            else { currentArgument.push_back(currentChar); }
            break;
        default:
            throw std::runtime_error("Unknown Parser-State");
    }
}
