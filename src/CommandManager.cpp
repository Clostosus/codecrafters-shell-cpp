#include <iostream>

#include "CommandManager.h"

#include "ArgumentsParser.h"
#include "SubprogramExecutor.h"
#include "FileSearcher.h"

void CommandManager::registerCommand(const Command& command) {
    commands.push_back(command);
}

const Command* CommandManager::getBuiltinCommand(const std::string& name) const {
    for (const auto& cmd : commands) {
        if (cmd.getName() == name) {  return &cmd; }
    }
    return nullptr; // Kein Command mit diesem Namen gefunden
}

void CommandManager::executeCommand(const std::string &name, std::vector<std::string> &arguments) const {
    if (const Command* cmd = getBuiltinCommand(name); cmd != nullptr) { // execute Builtin
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
    } catch (FileSearcher::FileNotFoundException &fileNotFoundError) { throw CommandNotFoundException(cmdName); }

    try {
        SubprogramExecutor executor(cmdName,args);
        const std::string result = executor.execute();
        if(!result.empty()){std::cout << result;}
    } catch (const SubprogramExecutor::SubprogramExecutorException &e) {
        std::cout  << e.what();
    }
}
