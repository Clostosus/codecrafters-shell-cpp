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
        if (cmd.getName() == name) {
            return &cmd;
        }
    }
    return nullptr; // Kein Command mit diesem Namen gefunden
}

void CommandManager::executeCommand(const std::string& name, const std::vector<std::string>& arguments) const {
    const Command* cmd = getBuiltinCommand(name);
    if (cmd != nullptr) {
        if (cmd->validateArguments(arguments)) {
            cmd->executeCommand(arguments);
        } else {
            throw std::runtime_error("Invalid arguments for command: " + name);
        }
    } else { // Search for executable File and execute it
        std::string pathToFile;
        try {
            FileSearcher searcher;
            pathToFile = searcher.getPathToFile(name);
        } catch (FileSearcher::FileNotFoundException &fileNotFoundError) {
            throw CommandNotFoundException(name);
        }

        try {
            SubprogramExecutor executor(pathToFile,arguments);
            const std::string result = executor.execute();
             if(!result.empty()){std::cout << result << std::endl;}
        } catch (const std::runtime_error &e) {
            std::cout << "Error while executing command: " << pathToFile << std::endl << e.what() << std::endl;
        }
    }
}
