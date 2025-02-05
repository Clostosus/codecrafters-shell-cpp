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

CommandOutput_t BuiltinCommand::executeWithoutRedirection(std::vector<std::string> &args) const {
    CommandOutput_t outp = executeFunction(args);
    return outp;
}


std::string BuiltinCommand::getDescription() const{
    return this->description;
}

std::string BuiltinCommand::getName() const{
    return this->name;
}

