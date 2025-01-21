#include <iostream>

#include "CommandManager.h"

#include "ArgumentsParser.h"
#include "SubprogramExecutor.h"
#include "FileSearcher.h"

void CommandManager::registerCommand(const Command& command) {
    if(commands.contains(command.getName())) {
        throw std::runtime_error("Command already exists: " + command.getName());
    } else {
        commands.insert(std::make_pair(command.getName(), command));
    }
}

const Command* CommandManager::getBuiltinCommand(const std::string& name) const {
    const auto it = commands.find(name);
    if(it != commands.end()) { return &it->second; }
    return nullptr; // Kein Command mit diesem Namen gefunden
}

void CommandManager::executeCommand(const std::string &name, std::vector<std::string> &arguments) const {
    const Command* cmd = getBuiltinCommand(name);
    if ( cmd != nullptr) { // execute Builtin
        if (cmd->validateArguments(arguments)) {
            cmd->executeCommand(arguments);
        } else { throw std::runtime_error("Invalid arguments for command: " + name); }
    } else { executeExternalBinary(name,arguments);}
}

void CommandManager::executeExternalBinary(const std::string& cmdName, const std::vector<std::string>& args) const{
    std::string pathToFile;
    try {
        FileSearcher searcher;
        pathToFile = searcher.getPathToFile(cmdName);
    } catch ([[maybe_unused]] FileSearcher::FileNotFoundException &fileNotFoundError) { throw CommandNotFoundException(cmdName); }

    try {
        SubprogramExecutor executor(cmdName,args);
        executor.execute();
    } catch (const SubprogramExecutor::SubprogramExecutorException &e) {
        std::cout  << e.what();
    }
}


void CommandManager::printAllCmdNames(std::ostringstream &outputstream) {
   auto it = commands.begin();
    while(it != commands.end()) {
        outputstream << it->first << std::endl;
        ++it;
    }
}
