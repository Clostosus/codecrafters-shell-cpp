#ifndef SUBPROGRAMEXECUTOR_H
#define SUBPROGRAMEXECUTOR_H

#include <string>
#include <vector>
#include <iostream>

class SubprogramExecutor {

public:
    SubprogramExecutor()=default;
    ~SubprogramExecutor()=default;
    // Executes a Subprogram with Arguments and waits until completion
    [[nodiscard]] std::string execute(std::string path,std::vector<std::string> args) const;
};


#endif //SUBPROGRAMEXECUTOR_H
