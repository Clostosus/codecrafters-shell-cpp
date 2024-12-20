#include "SubprogramExecutor.h"

#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

std::string SubprogramExecutor::execute(std::string path, std::vector<std::string> args) const {
    // Create an Argument-Array for exec
    std::vector<char*> execArgv;
    execArgv.push_back(path.data()); // Program name
    if (!args.empty()) {
        for (int i=0; i<args.size(); i++) {
            if(!args.at(i).empty()) {
                execArgv.push_back(args.at(i).data());
            }
        }
        execArgv.push_back(nullptr); // Null-terminated
    }


    // create Pipe "stringstream"
    int pipefd[2];
    if (pipe(pipefd) == -1) { throw std::runtime_error("Failed to create pipe"); }

    // Forks the current process
    pid_t pid = fork();
    if(pid == 0) { // child process
        close(pipefd[0]); // close pipe reading end

        //  (stdout -> pipe write-end)
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) { perror("dup2 failed"); _exit(EXIT_FAILURE); }
        close(pipefd[1]); // close Pipe writing end

        execvp(path.c_str(), execArgv.data());
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
            if (WEXITSTATUS(status) != 0) {
                throw std::runtime_error("Subprogram exited with error code: " + std::to_string(WEXITSTATUS(status)));
            }
        } else if (WIFSIGNALED(status)) {
            throw std::runtime_error("Subprogram terminated by signal: " + std::to_string(WTERMSIG(status)));
        }
        return output.str();
    } else { // fork failed
        throw std::runtime_error("Failed to fork process");
    }
    return "";
}
