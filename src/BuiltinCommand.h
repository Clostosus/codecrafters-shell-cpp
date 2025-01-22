#ifndef COMMAND_H
#define COMMAND_H
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "CommandInterface.h"

// Eigene Exception-Klasse für den Fehlerfall, dass keine Ausführungsfunktion definiert ist
class CommandExecutionException final : public std::runtime_error {
private:
    std::string commandName;

public:
    // Konstruktor mit dem Command-Namen
    explicit CommandExecutionException(const std::string& name)
        : std::runtime_error("Execution function not defined for command: " + name), commandName(name) {}

    // Zugriff auf den Command-Namen
    [[nodiscard]] std::string getCommandName() const {
        return commandName;
    }
};

struct CommandOutput_t {
    std::string stdoutOutput;
    std::string stderrOutput;
};

class BuiltinCommand : public CommandInterface{
protected:
    std::string name;
    std::string description;
    std::vector<std::string> argumentPrefixes;
    std::function<CommandOutput_t(const std::vector<std::string>& args)> executeFunction;
    std::function<bool(const std::vector<std::string>&)> validate;

    void executeBuiltinWithRedirect(const std::string &redirPath, const std::vector<std::string>& args, int rediredStream, bool append) const;
public:
    BuiltinCommand(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
            const std::function<CommandOutput_t(const std::vector<std::string> &args)> &execute,
            const std::function<bool(const std::vector<std::string> &)> &validate);

    BuiltinCommand(const std::string& name, const std::string& description, const std::function<CommandOutput_t(const std::vector<std::string>& args)> &execute);

    [[nodiscard]] bool validateArguments(const std::vector<std::string>& args) const;
    void execute(std::vector<std::string> &args) const override;

    [[nodiscard]] std::string getName()const override;
    [[nodiscard]] std::string getDescription()const override;
};


#endif //COMMAND_H
