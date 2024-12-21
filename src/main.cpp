#include <iostream>
#include <sstream>

#include "Command.h"
#include "CommandManager.h"
#include "FileSearcher.h"
#include "Builtins.h"
#include "CommandReader.h"


int main() {
  // Flush after every std::cout / std:cerr
  // std::cout << std::unitbuf; std::cerr << std::unitbuf;

  CommandManager manager;
  Builtins::registerBuiltinCommands(manager);

    // Read-Eval-Process Loop
    while (true) {
        std::cout << "$ ";
        try {
            std::string commandName;
            std::vector<std::string> arguments;
            CommandReader reader;
            reader.readOneLine(commandName,arguments);
            if (!commandName.empty()) { manager.executeCommand(commandName, arguments); }
        } catch (const CommandManager::CommandNotFoundException& e) {
            std::cout << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}
