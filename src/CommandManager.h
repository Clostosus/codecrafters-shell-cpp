#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "CommandInterface.h"

class CommandManager {
    protected:
    std::unordered_map<std::string, std::shared_ptr<CommandInterface>> commands;

public:
     void registerCommand(const std::shared_ptr<CommandInterface>& command);
     void executeCommand(const std::string& name, std::vector<std::string>& arguments) const;
    void printAllCmdNames(std::ostringstream & outputstream);
    std::vector<std::string> * getAllNamesWithPrefix(const std::string &prefix) const;
    bool existsBuiltinCommand(const std::string &name) const;
};

#endif //COMMANDMANAGER_H
