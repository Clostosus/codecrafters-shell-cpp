#include "Command.h"

Command::Command(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
    const std::function<void(const std::vector<std::string>& args)> & execute,const std::function<bool(const std::vector<std::string> &)> & validate) {
    this->name = name;
    this->description = description;
    this->argumentPrefixes = arguments;
    this->execute = execute;
    this->validate = validate;
}

Command::Command(const std::string& name, const std::string& description, const std::vector<std::string>& arguments) {
    this->name = name; this->description = description; this->argumentPrefixes = arguments;
    this->execute = nullptr; this->validate = nullptr;
}

// Überprüft, ob die Argumente gültig sind
bool Command::validateArguments(const std::vector<std::string>& args) const {
    return validate ? validate(args) : true; // Falls keine Validierungsfunktion vorhanden ist, gilt es als gültig.
}

void Command::executeCommand(const std::vector<std::string>& args) const {
    if (execute) {
        execute(args);
    } else {
        throw std::runtime_error("No execution function defined for this command.");
    }
}

std::string Command::getDescription() const{
    return this->description;
}

std::string Command::getName() const{
    return this->name;
}

