#include "CommandManager.h"

void CommandManager::registerCommand(const Command& command) {
    commands.push_back(command);
}

const Command* CommandManager::getCommand(const std::string& name) const {
    for (const auto& cmd : commands) {
        if (cmd.getName() == name) {
            return &cmd;
        }
    }
    return nullptr; // Kein Command mit diesem Namen gefunden
}

void CommandManager::executeCommand(const std::string& name, const std::vector<std::string>& arguments) const {
    const Command* cmd = getCommand(name);
    if (cmd == nullptr) { throw CommandNotFoundException(name); }

    if (cmd->validateArguments(arguments)) {
        cmd->executeCommand(arguments);
    } else {
        throw std::runtime_error("Invalid arguments for command: " + name);
    }
}
