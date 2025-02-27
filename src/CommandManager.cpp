#include <iostream>

#include "CommandManager.h"

#include "ArgumentsParser.h"
#include "BuiltinCommand.h"
#include "CommandNotFoundException.h"
#include "ExternalCommand.h"
#include "FileSearcher.h"

void CommandManager::registerCommand(const std::shared_ptr<CommandInterface>& command) {
    const std::string & name = command->getName();
    if(commands.contains(name)) {
        throw std::runtime_error("Command already exists: " + name);
    }
    commands.insert({name, command});
}

void CommandManager::executeCommand(const std::string &name, std::vector<std::string> &arguments) const {
    auto it = commands.find(name);
    if (it != commands.end()) {
        // execute Builtin-Command
        std::shared_ptr<BuiltinCommand> builtinCmd = dynamic_pointer_cast<BuiltinCommand>(it->second);
        if (builtinCmd->validateArguments(arguments)) {
            builtinCmd->execute(arguments);
        } else { throw std::runtime_error("Invalid arguments for command: " + name); }
    } else {
        try {
            const ExternalCommand external(name);
            external.execute(arguments);
        } catch (const CommandNotFoundException& e) {
            std::cout << e.what() << std::endl;
        }
    }
}


void CommandManager::printAllCmdNames(std::ostringstream &outputstream) {
   auto it = commands.begin();
    while(it != commands.end()) {
        outputstream << it->first << std::endl;
        ++it;
    }
}


bool CommandManager::existsBuiltinCommand(const std::string &name) const {
    if(const auto it = commands.find(name); it != commands.end()) {
        // Check if command is a builtin type command
        if (dynamic_cast<BuiltinCommand*>(it->second.get()) != nullptr) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> CommandManager::getAllBuiltinsWithPrefix(const std::string &prefix) const {
    std::vector<std::string> names;

    for (const auto& pair : this->commands) {
        const std::string& key = pair.first;
        if ( key.find(prefix) == 0) {
            names.push_back(key);
        }
    }
    return names;
}

std::vector<std::string> CommandManager::getAllExternalsWithPrefix(const std::string &prefix) {
    const FileSearcher * searcher = FileSearcher::getInstance();
    std::vector<std::string> AllNames = searcher->getExecutablesFromPath();
    std::vector<std::string> Matches;
    for(const std::string& name: AllNames) {
        if(name.find(prefix) == 0) {
            Matches.push_back(name);
        }
    }
    return Matches;
}
