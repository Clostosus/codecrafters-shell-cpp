// ReSharper disable CppRedundantElseKeywordInsideCompoundStatement
#include "SubprogramExecutor.h"

#include <complex>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

SubprogramExecutor::SubprogramExecutor(const std::string &cmdName, const std::vector<std::string> &args) {
    this->args = args; this->CmdName = cmdName;
    execArgv = std::vector<char *>();
}


void SubprogramExecutor::execute(){
    bool redirectRequired = false, append = false;
    std::string redirectPath;
    int redirectStream = 1;
    execArgv.push_back(CmdName.data()); // Program name
    if (!args.empty()) {
        for(int i=0; i < args.size(); i++) {
            if(!redirectRequired) {
                if(args.at(i) == ">" || args[i] == "1>"){redirectRequired = true; redirectStream = STDOUT_FILENO;}
                else if(args.at(i) == "2>"){redirectRequired = true; redirectStream = STDERR_FILENO;}
                else if(args[i] == ">>" || args[i] == "1>>"){redirectRequired = true; redirectStream = STDOUT_FILENO; append = true;}
                else if(args[i] == "2>>"){redirectRequired = true; redirectStream = STDERR_FILENO; append = true;}
                else if(!args[i].empty()) { execArgv.push_back(args.at(i).data()); }
            }else {
                redirectPath = args.at(i);
            }
        }
        execArgv.push_back(nullptr); // Null-terminated
    }
    if(redirectRequired) {
        executeWithRedirect(redirectPath, redirectStream,append);
    }else {
        try {
            std::cout << executeNoRedirect(execArgv);
        } catch (SubprogramExecutorException &e) {
            std::cout << e.what() << std::endl;
        }
    }
}

std::string SubprogramExecutor::executeNoRedirect(const std::vector<char *> & execArgv) const {
    // create Pipe "stringstream"
    int stdoutPipe[2],stderrPipe[2];
    if (pipe(stdoutPipe) == -1 || pipe(stderrPipe) == -1) { throw std::runtime_error("Failed to create pipe"); }

    // Forks the current process
    pid_t pid = fork();
    if(pid == 0) { // child process
        close(stdoutPipe[0]); close(stderrPipe[0]);// close pipes reading end

        //  (stdout -> stdoutPipe, stderr -> stderrPipe)
        if (dup2(stdoutPipe[1], STDOUT_FILENO) == -1 || dup2(stderrPipe[1], STDERR_FILENO) == -1) {
            perror("dup2 failed"); _exit(EXIT_FAILURE);
        }
        close(stdoutPipe[1]); close(stderrPipe[1]);// close Pipe writing end

        execvp(CmdName.c_str(), execArgv.data());
        // execvp only reaches this line on failure
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else if(pid > 0) { // parent process
        close(stdoutPipe[1]); close(stderrPipe[1]);// close Writing end of pipe
        // Read subprogram output
        std::ostringstream stdoutOutput, stderrOutput;
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer))) > 0) {
            stdoutOutput.write(buffer, bytesRead);
        }
        close(stdoutPipe[0]); // close Pipe Reading end

        // Read stderr
        while ((bytesRead = read(stderrPipe[0], buffer, sizeof(buffer))) > 0) {
            stderrOutput.write(buffer, bytesRead);
        }
        close(stderrPipe[0]);

        int status;
        waitpid(pid, &status, 0); // wait for child process to finish
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                throw SubprogramExecutorException("Subprogram exited with error code: " + std::to_string(WEXITSTATUS(status)));
            }
        } else if (WIFSIGNALED(status)) {
            throw SubprogramExecutorException("Subprogram terminated by signal: " + std::to_string(WTERMSIG(status)));
        }
        std::string result = stdoutOutput.str();
        return result;
    } else { // fork failed
        throw std::runtime_error("Failed to fork process");
    }
}

void SubprogramExecutor::executeWithRedirect(const std::string& pathToRedirectFile, int streamToRedirect, bool append) const {
    std::fstream file;
    if(append) {
        file.open(pathToRedirectFile,std::ios_base::app);
    }else {
        file.open(pathToRedirectFile, std::ios_base::out);
    }

    if(file.fail()){ file.close(); return;}

    // create Pipe "stringstream"
    int stdoutPipe[2],stderrPipe[2];
    if (pipe(stdoutPipe) == -1 || pipe(stderrPipe) == -1) { throw std::runtime_error("Failed to create pipe"); }

    // Forks the current process
    pid_t pid = fork();
    if(pid == 0) { // child process
        close(stdoutPipe[0]); close(stderrPipe[0]);// close pipes reading end

        //  (stdout -> stdoutPipe, stderr -> stderrPipe)
        if (dup2(stdoutPipe[1], STDOUT_FILENO) == -1 || dup2(stderrPipe[1], STDERR_FILENO) == -1) {
            perror("dup2 failed"); _exit(EXIT_FAILURE);
        }
        close(stdoutPipe[1]); close(stderrPipe[1]);// close Pipe writing end

        execvp(CmdName.c_str(), execArgv.data());
        // execvp only reaches this line on failure
        perror("execvp failed");
        _exit(EXIT_FAILURE);
    } else if(pid > 0) { // parent process
        close(stdoutPipe[1]); close(stderrPipe[1]);// close Writing end of pipe
        // Read subprogram output
        std::ostringstream stdoutOutput, stderrOutput;
        char buffer[1024];
        ssize_t bytesRead;
        // Read stdout
        if(streamToRedirect == STDOUT_FILENO) {
            while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer))) > 0) { file.write(buffer, bytesRead); }
        } else {
            while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer))) > 0) { stdoutOutput.write(buffer, bytesRead); }
        }
        close(stdoutPipe[0]); // close Pipe Reading end

        // Read stderr
        if(streamToRedirect == STDERR_FILENO) {
            while ((bytesRead = read(stderrPipe[0], buffer, sizeof(buffer))) > 0) { file.write(buffer, bytesRead); }
        } else {
            while ((bytesRead = read(stderrPipe[0], buffer, sizeof(buffer))) > 0) {stderrOutput.write(buffer, bytesRead);}
        }
        close(stderrPipe[0]);
        file.close();

        if(streamToRedirect != STDOUT_FILENO) {
            std::cout << stdoutOutput.str();
        }else { std::cout << stderrOutput.str(); }

        int status;
        waitpid(pid, &status, 0); // wait for child process to finish
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                // throw SubprogramExecutorException(stderrOutput.str());
            }
        } else if (WIFSIGNALED(status)) {
            throw SubprogramExecutorException("Subprogram terminated by signal: " + std::to_string(WTERMSIG(status)));
        } else { // fork failed
        throw std::runtime_error("Failed to fork process");
        }
    }
}

SubprogramExecutor::SubprogramExecutorException::SubprogramExecutorException(const std::string &errMessage): runtime_error(errMessage) {}

