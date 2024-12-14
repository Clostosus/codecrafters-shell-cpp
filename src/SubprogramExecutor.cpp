#include "SubprogramExecutor.h"

#include <sstream>

int SubprogramExecutor::execute(const std::string &path, const std::vector<std::string> &args) const {
    // Create an Argument-Array for exec
    std::vector<char *> execArgs;
    execArgs.push_back(const_cast<char *>(path.c_str())); // Program name
    for (const auto &arg : args) {
        execArgs.push_back(const_cast<char *>(arg.c_str()));
    }
    execArgs.push_back(nullptr); // Null-terminated

    // create Pipe "stringstream"
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error("Failed to create pipe");
    }

    // Forks the current process
    pid_t pid = fork();
    if(pid == 0) { // child process
        close(pipefd[0]); // close pipe reading end

        //  (stdout -> pipe write-end)
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed"); _exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // close Pipe writing end

        execvp(path.c_str(), execArgs.data());
        // execvp only reaches this line on failure
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else if(pid > 0) { // parent process
        close(pipefd[1]); // close Writing end of pipe
        // Read subprogram output
        std::ostringstream output;
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.write(buffer, bytesRead);
        }
        close(pipefd[0]); // close Pipe Reading end

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
