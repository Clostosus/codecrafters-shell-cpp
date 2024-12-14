#include "SubprogramExecutor.h"

int SubprogramExecutor::execute(const std::string &path, const std::vector<std::string> &args) const {
    // Create an Argument-Array for exec
    std::vector<char *> execArgs;
    execArgs.push_back(const_cast<char *>(path.c_str())); // Program name
    for (const auto &arg : args) {
        execArgs.push_back(const_cast<char *>(arg.c_str()));
    }
    execArgs.push_back(nullptr); // Null-terminated

    // Fork den aktuellen Prozess
    pid_t pid = fork();
    if(pid == 0) { // child process
        execvp(path.c_str(), execArgs.data());
        // execvp only reaches this line on failure
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else if(pid > 0) { // parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); // Rückgabewert des Subprogramms
        } else if (WIFSIGNALED(status)) {
            throw std::runtime_error("Subprogram terminated by signal: " + std::to_string(WTERMSIG(status)));
        }
    } else {
        throw std::runtime_error("Failed to fork process");
    }
    return -1; // This line shall never get reached
}
