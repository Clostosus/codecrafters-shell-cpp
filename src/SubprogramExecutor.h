#ifndef SUBPROGRAMEXECUTOR_H
#define SUBPROGRAMEXECUTOR_H

#include <string>
#include <vector>
#include <iostream>

class SubprogramExecutor {
private:
    std::vector<std::string> args;
    std::vector<char*> execArgv;
    std::string pathToCmd;
    [[nodiscard]] std::string executeNoRedirect(const std::vector<char *> & execArgv) const;
    void executeWithRedirect(const std::string& pathToRedirectFile) const;
public:
    SubprogramExecutor(const std::string &path, const std::vector<std::string> &args);
    ~SubprogramExecutor()=default;
    // Executes a Subprogram with Arguments and waits until completion
    [[nodiscard]] std::string execute();
};


#endif //SUBPROGRAMEXECUTOR_H
