#include <iostream>
#include <sstream>

#include "Command.h"
#include "CommandManager.h"
#include "FileSearcher.h"
#include "Builtins.h"


int main() {
  // Flush after every std::cout / std:cerr
  // std::cout << std::unitbuf; std::cerr << std::unitbuf;

  CommandManager manager;
  Builtins::registerBuiltinCommands(manager);

    // ReadEvalProcess-Loop
    while (true) {
        std::cout << "$ ";
        try {
            std::string input;
            std::getline(std::cin, input);

            if (!input.empty()) {
                // Command-Name extrahieren
                std::string cmdName = input.substr(0, input.find(' '));

                // Argumente extrahieren
                std::vector<std::string> arguments;
                std::stringstream inputStream(input.substr(input.find(' ') + 1));// +1 um führendes ' ' zu ignorieren
                std::string arg;
                while (inputStream >> arg) { arguments.push_back(arg); }
                manager.executeCommand(cmdName, arguments);
            }
        } catch (const CommandManager::CommandNotFoundException& e) {
            std::cout << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}
