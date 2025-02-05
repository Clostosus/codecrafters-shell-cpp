#ifndef COMMANDOUTPUT_H
#define COMMANDOUTPUT_H

#include <string>

struct CommandOutput_t {
    std::string stdoutOutput;
    std::string stderrOutput;

    CommandOutput_t() = default;

    CommandOutput_t(const std::string & stdout_output, const std::string & stderr_output) {
        stdoutOutput = stdout_output;
        stderrOutput = stderr_output;
    }
};

#endif //COMMANDOUTPUT_H
