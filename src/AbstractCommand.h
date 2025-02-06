#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <stdexcept>
#include <unistd.h>

#include "CommandInterface.h"
#include "CommandOutput.h"


struct RedirectionInfo {
    bool required = false;
    std::string redirectPath;
    int redirStream = STDOUT_FILENO;
    bool append = false;
};

// Own Exception in case a command has no defined execution function or execution failed.
class CommandExecutionException final : public std::runtime_error {
 std::string msg;
public:
    explicit CommandExecutionException(const std::string& message)
        : std::runtime_error( message) {msg = message;}
    [[nodiscard]] std::string getMessage() const {return msg;}
};

class AbstractCommand : public CommandInterface {
public:
    void execute(std::vector<std::string>& args) const override;

protected:
    virtual CommandOutput_t executeWithoutRedirection(std::vector<std::string>& args) const = 0;

    // Fills redirection info and removes the redirection arguments
    static RedirectionInfo parseRedirection(std::vector<std::string>& args);

    static void performRedirection(const RedirectionInfo& info, const CommandOutput_t& output);
};



#endif //ABSTRACTCOMMAND_H
