#include <iostream>
#include <sstream>

#include "Command.h"
#include "CommandManager.h"

void generateCommands(CommandManager & Manager) {
    // Beispiel-Command registrieren (Exit)
    Manager.registerCommand(Command(
        "exit",
        "Exits the console",
        {},
        [](const std::vector<std::string>& args) { std::exit(stoi(args.at(0))); },
        [](const std::vector<std::string>& args) {
            // Validierung des Arguments
              if (args.size() != 1) {
                  return false;  // Nur ein Argument sollte übergeben werden
              }

              // Überprüfen, ob das Argument eine gültige Zahl ist
              try {
                  int val = std::stoi(args[0]);
                  return true;  // Nur wenn das Argument int ist, gilt die Eingabe als gültig
              } catch (const std::invalid_argument& e) {
                  return false;  // Ungültiges Argument (keine gültige Zahl)
              } catch (const std::out_of_range& e) {
                  return false;  // Zahl ist außerhalb des gültigen Bereichs für int
              }
        }
    ));
    Manager.registerCommand(Command(
        "echo", "Prints the input", {},
        [](const std::vector<std::string>& args) { for (int i=0;i<args.size()-1;i++) { std::cout << args.at(i) << ' ';}
            std::cout << args.at(args.size()-1)<< std::endl; },
        [](const std::vector<std::string>& args) {
            return !args.empty();
        }
        ));
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // CommandManager erstellen
  CommandManager manager;
    generateCommands(manager);

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
