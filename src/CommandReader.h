#ifndef COMMANDREADER_H
#define COMMANDREADER_H
#include <string>
#include <vector>


class CommandReader {
   public:
   CommandReader();
   // Reads a line from stdin, fills Arguments vector and assigns cmdName, handles escape characters
   static void readOneLine(std::string & cmdName, std::vector<std::string> & arguments);
};



#endif //COMMANDREADER_H
