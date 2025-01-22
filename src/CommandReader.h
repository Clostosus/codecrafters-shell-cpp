#ifndef COMMANDREADER_H
#define COMMANDREADER_H
#include <string>
#include <vector>


class CommandReader {
private:
   enum ParserState {OutsideArgument, InsideWord, InsideSingleQuotes, InsideDoubleQuotes};
   ParserState currentState;
   bool escapedNextChar;
   void handleStateTransition(char currentChar, char nextChar, std::string& currentArgument);
   public:
   CommandReader();
   // Reads a line from stdin, fills Arguments vector and assigns cmdName, handles escape characters
   void readOneLine(std::string & cmdName, std::vector<std::string> & arguments);
};



#endif //COMMANDREADER_H
