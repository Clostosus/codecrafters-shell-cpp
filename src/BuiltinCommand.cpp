#include "BuiltinCommand.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <unistd.h>

BuiltinCommand::BuiltinCommand(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
                 const std::function<CommandOutput_t(const std::vector<std::string>& args)> & execute,const std::function<bool(const std::vector<std::string> &)> & validate) {
    this->name = name;
    this->description = description;
    this->argumentPrefixes = arguments;
    this->executeFunction = execute;
    this->validate = validate;
}

BuiltinCommand::BuiltinCommand(const std::string& name, const std::string& description, const std::function<CommandOutput_t(const std::vector<std::string>& args)> &execute) {
    this->name = name; this->description = description; this->argumentPrefixes = std::vector<std::string>();
    this->executeFunction = execute; this->validate = nullptr;
}

bool BuiltinCommand::validateArguments(const std::vector<std::string>& args) const {
    return validate ? validate(args) : true; // If there is no validate function, it should execute.
}

void BuiltinCommand::execute( std::vector<std::string>& args) const {
    if (! executeFunction) {throw std::runtime_error("No execution function defined for this command.");}
    bool redirectRequired = false, append = false;
    std::string redirectPath; int redirStream = 1;
   if (!args.empty()) {
     int breakNum = -1;
     for (int i=0; i < args.size(); i++) {
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
      auto [stdoutOutput, stderrOutput] = executeFunction(args);
      if (!stdoutOutput.empty()){ std::cout << stdoutOutput; }
      if(!stderrOutput.empty()){ std::cout << stderrOutput; }
   }
}

void BuiltinCommand::executeBuiltinWithRedirect(const std::string &redirPath, const std::vector<std::string> &args, int rediredStream, bool append) const {
    std::fstream file;
    if(append) {
        file.open(redirPath, std::ios::app);
    } else {
        file.open(redirPath, std::ios_base::out);
    }

    if(!file.fail()) {
        auto [stdoutOutput, stderrOutput] = executeFunction(args);
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

std::string BuiltinCommand::getDescription() const{
    return this->description;
}

std::string BuiltinCommand::getName() const{
    return this->name;
}

