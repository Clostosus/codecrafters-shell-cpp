#include "CommandEvaluator.h"

CommandEvaluator::CommandEvaluator() {
    this->validCommands = std::vector<std::string>();
    validCommands.emplace_back("echo");
    validCommands.emplace_back("exit");
    validCommands.emplace_back("cd");
}

CommandEvaluator::~CommandEvaluator() {
     validCommands.~vector();
}

bool CommandEvaluator::isValidCommand(const std::string& Command) {
    for (const std::string & cmdName: validCommands) {
        if(Command == cmdName) {return true;}
    }
    return false;
}

void CommandEvaluator::addValidCommand(const std::string& Command) {
    validCommands.emplace_back(Command);
}
