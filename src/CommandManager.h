#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <vector>
#include <string>
#include "Command.h"

class CommandManager {
    protected:
    std::unordered_map<std::string, Command> commands;
    void executeExternalBinary(const std::string& cmdName, const std::vector<std::string>& args)const;

public:
     class CommandNotFoundException final : public std::runtime_error {
     public:
        explicit CommandNotFoundException(const std::string& name)
            : std::runtime_error( name + ": command not found") {}
     };

     void registerCommand(const Command& command);
     [[nodiscard]] const Command* getBuiltinCommand(const std::string& name) const;
     void executeCommand(const std::string& name, std::vector<std::string>& arguments) const;
    void printAllCmdNames(std::ostringstream & outputstream);
};

#endif //COMMANDMANAGER_H
