#include "Builtins.h"

#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

#include "FileSearcher.h"
#include "CommandNotFoundException.h"


void Builtins::registerBuiltinCommands(CommandManager & Manager) {
    // Beispiel-Command registrieren (Exit)
    Manager.registerCommand(std::make_shared<BuiltinCommand>(BuiltinCommand(
        "exit",
        "Exits the console",
        {},
        [](const std::vector<std::string>& args) -> CommandOutput_t {CommandOutput_t output; std::exit(stoi(args.at(0))); },
        [](const std::vector<std::string>& args) {
            // Validierung des Arguments
              if (args.size() != 1) {
                  return false;  // Nur ein Argument sollte übergeben werden
              }

              // Überprüfen, ob das Argument eine gültige Zahl ist
              try {
                  std::stoi(args[0]);
                  return true;  // Nur wenn das Argument int ist, gilt die Eingabe als gültig
              } catch (const std::invalid_argument& ) {
                  return false;  // Ungültiges Argument (keine gültige Zahl)
              } catch (const std::out_of_range& ) {
                  return false;  // Zahl ist außerhalb des gültigen Bereichs für int
              }
        }
    )));
    Manager.registerCommand(std::make_shared<BuiltinCommand>(BuiltinCommand(
        "echo", "Prints the input", {},
        [](const std::vector<std::string>& args) -> CommandOutput_t
        {
            CommandOutput_t output;
            std::ostringstream oss;
            for (int i=0;i<args.size()-1;i++) { oss << args.at(i)<< ' '; }
            oss << args.at(args.size()-1)<< std::endl;
            output.stdoutOutput = oss.str(); output.stderrOutput = "";
            return output;
        },
        [](const std::vector<std::string>& args) {
            return !args.empty();
        }
        )));
    Manager.registerCommand(std::make_shared<BuiltinCommand>(BuiltinCommand(
        "pwd", "Print the current working directory",
        [](const std::vector<std::string>& args)->CommandOutput_t {
            CommandOutput_t output;
            std::ostringstream oss;
            char pathBuffer[1024];
            if(getcwd(pathBuffer, sizeof(pathBuffer)) != nullptr) {
                oss << pathBuffer << std::endl; output.stdoutOutput = oss.str(); output.stderrOutput = "";
            } else {
                oss << strerror(errno) << std::endl; output.stderrOutput = oss.str(); output.stdoutOutput = "";
            }
            return output;
        })));
    Manager.registerCommand(std::make_shared<BuiltinCommand>(BuiltinCommand(
        "cd", "Change current working directory",{},
        [](const std::vector<std::string>& args)->CommandOutput_t {
            CommandOutput_t output;
            std::string targetPath = args.at(0); // create a modifiable copy
            const auto pos = targetPath.find('~'); // handle user home path
               if(pos != std::string::npos) {
                   targetPath.replace(pos,1,getenv("HOME"));
               }

           std::ostringstream oss;
           struct stat statStruct{};
           // If the file/directory exists at the path returns 0
           if (stat(targetPath.c_str(), &statStruct) == 0) {
               chdir(targetPath.c_str()); output.stdoutOutput = ""; output.stderrOutput = "";
           }else {
               output.stderrOutput = "cd: " + targetPath + ": No such file or directory" + '\n';
           }
           return output;
        }, [](const std::vector<std::string>& args) {
            if(args.empty()) return false;
            return true;
        })));
    Manager.registerCommand(std::make_shared<BuiltinCommand>(BuiltinCommand(
        "type", "Prints the type of a command name",{},
        [Manager](const std::vector<std::string>& args) ->CommandOutput_t {
            CommandOutput_t output;
            std::ostringstream oss;
            if (args.size() != 1) {
                oss << "Usage: type <commandName>" << std::endl;
                output.stderrOutput = oss.str(); output.stdoutOutput = "";
                return output;
            }
            const std::string& searchedCmdName = args.at(0);

            try {
                if(!Manager.existsBuiltinCommand(searchedCmdName)) {
                    throw CommandNotFoundException(args.at(0));
                }
                oss << searchedCmdName << " is a shell builtin"   << std::endl;
            } catch (CommandNotFoundException&) {
                if(args.at(0) != "type") {
                  auto searcher = FileSearcher();
                  try {
                      std::string exePath = searcher.getPathToFile(searchedCmdName);
                      oss << searchedCmdName << " is " << exePath << std::endl;
                  } catch (FileSearcher::FileNotFoundException &filefindError) {
                      oss << filefindError.what() << ": not found" << std::endl;
                  }
                }else {
                    oss << "type" << " is a shell builtin"   << std::endl;
                }
            }
            output.stdoutOutput = oss.str();
            return output;
        },
        [](const std::vector<std::string>& args) {return args.size() == 1; }
        )));
}
