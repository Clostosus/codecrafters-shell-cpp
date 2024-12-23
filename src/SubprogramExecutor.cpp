// ReSharper disable CppRedundantElseKeywordInsideCompoundStatement
#include "SubprogramExecutor.h"

#include <complex>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

SubprogramExecutor::SubprogramExecutor(const std::string &path, const std::vector<std::string> &args) {
    this->args = args; this->pathToCmd = path;
    execArgv = std::vector<char *>();
}


std::string SubprogramExecutor::execute(){
    bool redirectRequired = false; std::string redirectPath;
    execArgv.push_back(pathToCmd.data()); // Program name
    if (!args.empty()) {
        int i = 0;
        for (i=0; i < args.size(); i++) {
            if(!redirectRequired) {
                if(args.at(i) == ">" || args[i] == "1>"){redirectRequired = true;}
                else if(!args.at(i).empty()) { execArgv.push_back(args.at(i).data()); }
            }else {
                redirectPath = args.at(i);
            }
        }
        execArgv.push_back(nullptr); // Null-terminated
    }
    if(redirectRequired) {
        executeWithRedirect(redirectPath);
        return "";
    }else {
        try {
            return executeNoRedirect(execArgv);
        } catch (SubprogramExecutorException &e) {
            std::cout << e.what() << std::endl;
            return "";
        }
    }
}

std::string SubprogramExecutor::executeNoRedirect(const std::vector<char *> & execArgv) const {
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

        execvp(pathToCmd.c_str(), execArgv.data());
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
        waitpid(pid, &status, 0); // wait for child process to finish
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                throw SubprogramExecutorException("Subprogram exited with error code: " + std::to_string(WEXITSTATUS(status)));
            }
        } else if (WIFSIGNALED(status)) {
            throw SubprogramExecutorException("Subprogram terminated by signal: " + std::to_string(WTERMSIG(status)));
        }
        std::string result = output.str();
        return result;
    } else { // fork failed
        throw std::runtime_error("Failed to fork process");
    }
}

void SubprogramExecutor::executeWithRedirect(const std::string& pathToRedirectFile) const {
     auto output = std::ostringstream(executeNoRedirect(execArgv));
     std::ofstream file;
     file.open (pathToRedirectFile);
     file.write(output.str().c_str(), output.str().size());
     file.close();
}

SubprogramExecutor::SubprogramExecutorException::SubprogramExecutorException(const std::string &errMessage): runtime_error(errMessage) {}

