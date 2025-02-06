#ifndef COMMAND_H
#define COMMAND_H
#include <functional>
#include <string>
#include <vector>

#include "AbstractCommand.h"
#include "CommandOutput.h"


class BuiltinCommand : public AbstractCommand{
protected:
    std::string name;
    std::string description;
    std::vector<std::string> argumentPrefixes;
    std::function<CommandOutput_t(const std::vector<std::string>& args)> executeFunction;
    std::function<bool(const std::vector<std::string>&)> validate;

    CommandOutput_t executeWithoutRedirection(std::vector<std::string> &args) const override;
public:
    BuiltinCommand(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
            const std::function<CommandOutput_t(const std::vector<std::string> &args)> &execute,
            const std::function<bool(const std::vector<std::string> &)> &validate);

    BuiltinCommand(const std::string& name, const std::string& description, const std::function<CommandOutput_t(const std::vector<std::string>& args)> &execute);

    [[nodiscard]] bool validateArguments(const std::vector<std::string>& args) const;

    [[nodiscard]] std::string getName()const override;
    [[nodiscard]] std::string getDescription()const;
};


#endif //COMMAND_H
