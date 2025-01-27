#ifndef SUBPROGRAMEXECUTOR_H
#define SUBPROGRAMEXECUTOR_H

#include <string>
#include <vector>
#include <iostream>

class SubprogramExecutor {
protected:
    std::vector<std::string> args;
    std::vector<char*> execArgv;
    std::string CmdName;
    [[nodiscard]] std::string executeNoRedirect(const std::vector<char *> & execArgv) const;
    void executeWithRedirect(const std::string& pathToRedirectFile, int streamToRedirect, bool append) const;
public:
    SubprogramExecutor(const std::string &cmdName, const std::vector<std::string> &args);
    ~SubprogramExecutor()=default;
    // Executes a Subprogram with Arguments and waits until completion
    void execute();

    class SubprogramExecutorException final : public std::runtime_error {
    protected:
        std::string errorMessage;
        public:
        explicit SubprogramExecutorException(const std::string &errMessage);
    };
};


#endif //SUBPROGRAMEXECUTOR_H
