// ReSharper disable All
#include "CommandReader.h"

#include <iostream>
#include <sstream>

#define END '\0'
#define SPACE ' '
#define SINGLE '\''
#define DOUBLE '\"'
#define BACKSLASH '\\'
#define TAB '\t'
#define NEWLINE '\n'

CommandReader::CommandReader() {
    currentState = ParserState::OutsideArgument;
    escapedNextChar = false;
};

void CommandReader::readOneLine(std::string &cmdName, std::vector<std::string> &arguments, CommandManager &cmdManager) {
    arguments.clear(); cmdName.clear();
    std::string input;
    readCharacterByCharacter(input, cmdManager);
    if (input.empty()) {cmdName = ""; return;}

    // extract Arguments from line
    std::string arg;
    std::string::iterator pos = input.begin();

    for (pos ; pos <= input.end()+1; pos++) {
        char c = *pos, nextChar = *(pos+1);
        this->handleStateTransition(input, pos, arg);
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
    while(true) {
        c = static_cast<char>(getchar());
        if(c == NEWLINE) {
            std::cout << NEWLINE << std::flush;
            break;
        } else if (c == TAB) { // Autocompletion for Tab Key
            autoComplete(currentInput, manager);
        } else{
            currentInput += c;
            std::cout << c << std::flush;
        }
    };
}

void CommandReader::autoComplete(std::string &currentInput, CommandManager &manager) {
    std::vector<std::string> * builtinSuggestions = manager.getAllBuiltinsWithPrefix(currentInput);
    if (builtinSuggestions && builtinSuggestions->size() == 1) {
        currentInput = builtinSuggestions->front() + ' ';
        std::cout << "\r$ " << currentInput << std::flush;
    }else if(builtinSuggestions->empty()) {
        std::vector<std::string> * externalSuggestions = manager.getAllExternalsWithPrefix(currentInput);
        if (externalSuggestions && externalSuggestions->size() == 1) {
            currentInput = externalSuggestions->front() + ' ';
            std::cout << "\r$ " << currentInput << std::flush;
        }else if(externalSuggestions->size() > 1) {
            std::cout << '\n' << "\r" << std::flush;
            for (std::string & suggestion : *externalSuggestions ) {
                if(suggestion.empty()) {continue;}
                std::cout << suggestion << ' ';
            }
            std::cout << '\n' << "\r$" << currentInput << std::flush;
        }else{
            std::cout << '\a' << std::flush;
        }
        delete externalSuggestions;
    }
    delete builtinSuggestions;
}



void CommandReader::handleStateTransition(const std::string &inputLine, std::string::iterator pos, std::string &currentArgument) {

    char previousChar = '\0';
    if (pos != inputLine.begin()) { previousChar = *(pos - 1); }
    char currentChar = static_cast<char>(*pos);
    char nextChar = static_cast<char>(*(pos+1));

    if(currentState != ParserState::InsideSingleQuotes) {
        if(escapedNextChar) {
            escapedNextChar = false; currentArgument.push_back(currentChar);
            return;
        }
    }

    switch (currentState) {
      case ParserState::InsideSingleQuotes:
          if(currentChar == SINGLE) {
              if(nextChar == SINGLE){currentState == InsideSingleQuotes;}
              else if(previousChar == SINGLE) {
                  currentState = ParserState::InsideSingleQuotes;
              } else{
                  if(nextChar == SPACE || nextChar == END){currentState = OutsideArgument;}
                  else{currentState = InsideWord;}
              }
          } else {
              currentArgument.push_back(currentChar);
          }
          break;
        case ParserState::InsideWord:
            if (currentChar == SPACE ) {
                currentState = ParserState::OutsideArgument;// Argument finished
            } else if (currentChar == SINGLE) {
                currentState = ParserState::InsideSingleQuotes;
            } else if(currentChar == END ) {
                currentState = ParserState::OutsideArgument;
            }else if (currentChar == BACKSLASH) {
                escapedNextChar = true;
            } else {
                currentArgument.push_back(currentChar);
            }
            break;
        case ParserState::OutsideArgument:
            if (currentChar == SINGLE) {
                currentState = ParserState::InsideSingleQuotes;
            } else if (currentChar == END) {

            } else if (currentChar == DOUBLE) {
                currentState = ParserState::InsideDoubleQuotes;
            } else if(currentChar == BACKSLASH) {
                if(nextChar == BACKSLASH || nextChar == '$' || nextChar == SINGLE || nextChar == DOUBLE || nextChar == SPACE) {
                    escapedNextChar = true;
                    currentState = ParserState::InsideWord; // Übergang erzwingen, damit der nächste Buchstabe zum Wort gehört
                }
                else{currentArgument.push_back(currentChar);}
            }else if(currentChar != SPACE) {
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
                if(nextChar == DOUBLE){currentState == InsideDoubleQuotes;}
                else if(previousChar == DOUBLE) {
                    currentState = ParserState::InsideDoubleQuotes;
                } else{
                    if(nextChar == SPACE || nextChar == END){currentState = OutsideArgument;}
                    else{currentState = InsideWord;}
                }
            }
            else { currentArgument.push_back(currentChar); }
            break;
        default:
            throw std::runtime_error("Unknown Parser-State");
    }
}
