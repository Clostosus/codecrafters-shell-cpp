#include "Command.h"

#include <fstream>
#include <iostream>
#include <ostream>

Command::Command(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
                 const std::function<std::string(const std::vector<std::string>& args)> & execute,const std::function<bool(const std::vector<std::string> &)> & validate) {
    this->name = name;
    this->description = description;
    this->argumentPrefixes = arguments;
    this->execute = execute;
    this->validate = validate;
}

Command::Command(const std::string& name, const std::string& description, const std::function<std::string(const std::vector<std::string>& args)> &execute) {
    this->name = name; this->description = description; this->argumentPrefixes = std::vector<std::string>();
    this->execute = execute; this->validate = nullptr;
}

// Überprüft, ob die Argumente gültig sind
bool Command::validateArguments(const std::vector<std::string>& args) const {
    return validate ? validate(args) : true; // Falls keine Validierungsfunktion vorhanden ist, gilt es als gültig.
}

void Command::executeCommand( std::vector<std::string>& args) const {
    if (execute) {
        bool redirectRequired = false; std::string redirectPath;
        if (!args.empty()) {
            int i, breakNum = -1;
            for (i=0; i < args.size(); i++) {
                if(!redirectRequired) {
                    if(args.at(i) == ">" || args[i] == "1>"){redirectRequired = true; breakNum = i;}
                }else {
                    redirectPath = args.at(i);
                }
            }
            if(breakNum != -1) {
                for(int j = static_cast<int>(args.size())-1; j >= breakNum; j--) {
                    args.erase(args.begin()+j);
                }
            }
        }

        if(redirectRequired) {
            executeBuiltinWithRedirect(redirectPath,args);
        }else {
            std::string result = execute(args);
            if (!result.empty()){ std::cout << result; }
        }

    } else {
        throw std::runtime_error("No execution function defined for this command.");
    }
}

void Command::executeBuiltinWithRedirect(const std::string &redirPath, const std::vector<std::string> &args) const {
    const auto output = this->execute(args);
    std::fstream file;
    file.open(redirPath, std::ios_base::out);
    if(!file.fail()) {
        file.write(output.c_str(), static_cast<long>(output.size()));
    }
    file.close();
}

std::string Command::getDescription() const{
    return this->description;
}

std::string Command::getName() const{
    return this->name;
}

