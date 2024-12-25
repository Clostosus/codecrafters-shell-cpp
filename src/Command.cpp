#include "Command.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <unistd.h>

Command::Command(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
                 const std::function<CommandOutput_t(const std::vector<std::string>& args)> & execute,const std::function<bool(const std::vector<std::string> &)> & validate) {
    this->name = name;
    this->description = description;
    this->argumentPrefixes = arguments;
    this->execute = execute;
    this->validate = validate;
}

Command::Command(const std::string& name, const std::string& description, const std::function<CommandOutput_t(const std::vector<std::string>& args)> &execute) {
    this->name = name; this->description = description; this->argumentPrefixes = std::vector<std::string>();
    this->execute = execute; this->validate = nullptr;
}

// Überprüft, ob die Argumente gültig sind
bool Command::validateArguments(const std::vector<std::string>& args) const {
    return validate ? validate(args) : true; // Falls keine Validierungsfunktion vorhanden ist, gilt es als gültig.
}

void Command::executeCommand( std::vector<std::string>& args) const {
    if (! execute) {throw std::runtime_error("No execution function defined for this command.");}
    bool redirectRequired = false, append = false;
    std::string redirectPath; int redirStream = 1;
   if (!args.empty()) {
     int i, breakNum = -1;
     for (i=0; i < args.size(); i++) {
      if(!redirectRequired) {
       if(args.at(i) == ">" || args[i] == "1>"){redirectRequired = true; redirStream = STDOUT_FILENO; breakNum = i;}
       else if(args[i] == "2>"){redirectRequired = true; redirStream = STDERR_FILENO; breakNum = i; }
       else if(args[i] == "1>>" || args[i] == ">>"){redirectRequired = true; redirStream = STDOUT_FILENO; breakNum = i; append = true;}
       else if(args[i] == "2>>"){redirectRequired = true, redirStream = STDERR_FILENO; breakNum = i; append = true;}
      }else {redirectPath = args.at(i);}
     }
     if(breakNum != -1) { // remove redirect and all following from arguments
      for(int j = static_cast<int>(args.size())-1; j >= breakNum; j--) {
       args.erase(args.begin()+j);
      }
     }
   }
   if(redirectRequired) {
      executeBuiltinWithRedirect(redirectPath,args, redirStream,append);
   }else {
      auto [stdoutOutput, stderrOutput] = execute(args);
      if (!stdoutOutput.empty()){ std::cout << stdoutOutput; }
      if(!stderrOutput.empty()){ std::cout << stderrOutput; }
   }
}

void Command::executeBuiltinWithRedirect(const std::string &redirPath, const std::vector<std::string> &args, int rediredStream, bool append) const {
    std::fstream file;
    if(append) {
        file.open(redirPath, std::ios::app);
    } else {
        file.open(redirPath, std::ios_base::out);
    }

    if(!file.fail()) {
        auto [stdoutOutput, stderrOutput] = execute(args);
        switch(rediredStream) {
            case STDOUT_FILENO:
                file.write(stdoutOutput.c_str(), static_cast<long>(stdoutOutput.size()));
                std::cerr << stderrOutput;
                break;
            case STDERR_FILENO:
                file.write(stderrOutput.c_str(), static_cast<long>(stderrOutput.size()));
                std::cout << stdoutOutput;
                break;
            default:
                throw std::runtime_error("Invalid redirection stream.");
        }
    }
    file.close();
}

std::string Command::getDescription() const{
    return this->description;
}

std::string Command::getName() const{
    return this->name;
}

