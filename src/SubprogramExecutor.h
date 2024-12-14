#ifndef SUBPROGRAMEXECUTOR_H
#define SUBPROGRAMEXECUTOR_H

#include <unistd.h> // Für fork, exec
#include <sys/types.h>
#include <sys/wait.h> // Für waitpid
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

class SubprogramExecutor {

public:
    SubprogramExecutor()=default;
    ~SubprogramExecutor()=default;
    // Executes a Subprogram with Arguments and waits until completion
    [[nodiscard]] int execute(const std::string &path, const std::vector<std::string> &args) const;
};


#endif //SUBPROGRAMEXECUTOR_H
