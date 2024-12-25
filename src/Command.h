#ifndef COMMAND_H
#define COMMAND_H
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

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

class Command {
protected:
    std::string name;
    std::string description;
    std::vector<std::string> argumentPrefixes;
    std::function<CommandOutput_t(const std::vector<std::string>& args)> execute;
    std::function<bool(const std::vector<std::string>&)> validate;

    void executeBuiltinWithRedirect(const std::string &redirPath, const std::vector<std::string>& args, int rediredStream, bool append) const;
public:
    Command(const std::string &name, const std::string &description, const std::vector<std::string> &arguments,
            const std::function<CommandOutput_t(const std::vector<std::string> &args)> &execute,
            const std::function<bool(const std::vector<std::string> &)> &validate);

    Command(const std::string& name, const std::string& description, const std::function<CommandOutput_t(const std::vector<std::string>& args)> &execute);

    [[nodiscard]] bool validateArguments(const std::vector<std::string>& args) const;
    void executeCommand(std::vector<std::string> &args) const;

    [[nodiscard]] std::string getName()const;
    [[nodiscard]] std::string getDescription()const;
};


#endif //COMMAND_H
