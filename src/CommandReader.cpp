// ReSharper disable All
#include "CommandReader.h"

#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>

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
    AlreadyPressedTab = false;
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
    std::vector<std::string> suggestions = collectAllSuggestions(currentInput, manager);

    if (suggestions.size() == 1) {
            currentInput = suggestions.front() + ' ';
            std::cout << "\r$ " << currentInput << std::flush;
    }else if(suggestions.size() > 1) {
        if(!AlreadyPressedTab) {
            std::cout << '\a' << std::flush;
            AlreadyPressedTab = true;
        }else {
            int i=0;
            std::cout << '\n' << "\r" << std::flush;
            for (i=0; i < suggestions.size()-1; i++) {
                std::cout << suggestions.at(i) << ' ' << ' ';
            }
            std::cout << suggestions.at(i) << std::endl;
            std::cout << "\r$ " << currentInput << std::flush;
            AlreadyPressedTab = false;
        }
    }else if(suggestions.empty()){
            std::cout << '\a' << std::flush;
    }
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
            if (currentChar == SPACE || currentChar == END) {
                currentState = ParserState::OutsideArgument;// Argument finished
            } else if (currentChar == SINGLE) {
                currentState = ParserState::InsideSingleQuotes;
            } else if (currentChar == BACKSLASH) {
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

std::vector<std::string> CommandReader::collectAllSuggestions(std::string &currentInput,CommandManager &manager) {
    std::set<std::string> AllSuggestions;
    std::vector<std::string> builtinSuggestions = manager.getAllBuiltinsWithPrefix(currentInput);
    std::vector<std::string> externalSuggestions = manager.getAllExternalsWithPrefix(currentInput);

    for(const auto & builtinSuggestion : builtinSuggestions) {
        AllSuggestions.insert(builtinSuggestion);
    }
    for(const auto & externalSuggestion : externalSuggestions) {
        if(!AllSuggestions.contains(externalSuggestion)) {
          AllSuggestions.insert(externalSuggestion);
        }
    }
    return std::vector(AllSuggestions.begin(), AllSuggestions.end());
}
