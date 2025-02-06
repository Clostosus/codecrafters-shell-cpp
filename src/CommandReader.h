#ifndef COMMANDREADER_H
#define COMMANDREADER_H
#include <string>
#include <vector>

#include "CommandManager.h"


class CommandReader {
protected:
   enum ParserState {OutsideArgument, InsideWord, InsideSingleQuotes, InsideDoubleQuotes};
   ParserState currentState;
   bool escapedNextChar;
   void handleStateTransition(const std::string &inputLine,std::string::iterator pos, std::string& currentArgument);
   static void readCharacterByCharacter( std::string &currentInput, CommandManager &manager);
   static void autoComplete(std::string &currentInput, CommandManager &manager);
public:
   CommandReader();
   // Reads a line from stdin, fills Arguments vector and assigns cmdName, handles escape characters
   void readOneLine(std::string & cmdName, std::vector<std::string> & arguments, CommandManager &manager);
};



#endif //COMMANDREADER_H
