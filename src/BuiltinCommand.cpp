#include "BuiltinCommand.h"

#include <fstream>

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

