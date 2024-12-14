#include <iostream>

#include "CommandManager.h"
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
        SubprogramExecutor executor;
        FileSearcher searcher;
        try {
            searcher.getPathToFile(name);
        } catch (FileSearcher::FileNotFoundException &fileNotFoundError) {
            throw CommandNotFoundException(name);
        }

        try {
            int returnCode = executor.execute("/bin/ls", {"-l", "-a"});
            std::cout << "Subprogram exited with code: " << returnCode << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
