#ifndef COMMANDEVALUATOR_H
#define COMMANDEVALUATOR_H

#include <string>
#include <vector>

class CommandEvaluator {
private:
   std::vector<std::string> validCommands;
public:
   CommandEvaluator();
   ~CommandEvaluator();
   bool isValidCommand(const std::string& Command);
   void addValidCommand(const std::string& Command);

};



#endif //COMMANDEVALUATOR_H
