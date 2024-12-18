#include "Builtins.h"

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <bits/ranges_algo.h>
#include <sys/stat.h>

#include "FileSearcher.h"


void Builtins::registerBuiltinCommands(CommandManager & Manager) {
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
    Manager.registerCommand(Command("pwd", "Print the current working directory",
        [](const std::vector<std::string>& args) {
            char pathBuffer[1024];
            if(getcwd(pathBuffer, sizeof(pathBuffer)) != nullptr) {
                std::cout << pathBuffer << std::endl;
            } else {
                std::cout << strerror(errno) << std::endl;
            }
        }));
    Manager.registerCommand(Command("cd", "Change current working directory",
        {},
        [](const std::vector<std::string>& args) {
            std::string targetPath = args.at(0); // create a modifiable copy
            const auto pos = targetPath.find('~'); // handle user home path
               if(pos != std::string::npos) {
                   targetPath.replace(pos,1,getenv("HOME"));
               }

           struct stat statStruct{};
           // If the file/directory exists at the path returns 0
           if (stat(targetPath.c_str(), &statStruct) == 0) {
               chdir(targetPath.c_str());
           }else {
                   std::cout << "cd: " << targetPath<< ": No such file or directory" << std::endl; return;
           }
        }, [](const std::vector<std::string>& args) {
            if(args.empty()) return false;
            return true;
        }));
    Manager.registerCommand(Command("type", "Prints the type of a command name",{},
        [Manager](const std::vector<std::string>& args) {
            if (args.size() != 1) { std::cout << "Usage: type <commandName>" << std::endl; return; }
            const std::string& searchedCmdName = args.at(0);

            try {
                if(Manager.getBuiltinCommand(searchedCmdName) == nullptr) {
                    throw CommandManager::CommandNotFoundException(args.at(0));
                }
                std::cout << searchedCmdName << " is a shell builtin"   << std::endl;
            } catch (CommandManager::CommandNotFoundException &e) {
                if(args.at(0) != "type") {
                  auto searcher = FileSearcher();
                  try {
                      std::string exePath = searcher.getPathToFile(searchedCmdName);
                      std::cout << searchedCmdName << " is " << exePath << std::endl;
                  } catch (FileSearcher::FileNotFoundException &filefindError) {
                      std::cout << filefindError.what() << ": not found" << std::endl;
                  }

                }else {
                    std::cout << "type" << " is a shell builtin"   << std::endl;
                }
            }
        },
        [](const std::vector<std::string>& args) {return args.size() == 1; }
        ));
}
