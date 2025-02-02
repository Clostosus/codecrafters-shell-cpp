#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <fstream>
#include <iostream>
#include <unistd.h>

#include "CommandInterface.h"
#include "CommandOutput.h"


struct RedirectionInfo {
    bool required = false;
    std::string redirectPath;
    int redirStream = STDOUT_FILENO;
    bool append = false;
};


class AbstractCommand : public CommandInterface {
public:
    void execute(std::vector<std::string>& args) const override {
        const RedirectionInfo redirInfo = parseRedirection(args); // redir parameters get removed from args while parsing

        const CommandOutput_t output = runWithoutRedirection(args);

        if(redirInfo.required) {
            performRedirection(redirInfo, output);
        } else {
            if(!output.stdoutOutput.empty()) { std::cout << output.stdoutOutput; }
            if(!output.stderrOutput.empty()) { std::cerr << output.stderrOutput; }
        }
    }

protected:
    virtual CommandOutput_t runWithoutRedirection(std::vector<std::string>& args) const = 0;

    // Fills redirection info and removes the redirection arguments
    static RedirectionInfo parseRedirection(std::vector<std::string>& args);

    static void performRedirection(const RedirectionInfo& info, const CommandOutput_t& output);
};



#endif //ABSTRACTCOMMAND_H
