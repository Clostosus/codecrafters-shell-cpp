#include "ExternalCommand.h"

#include <sys/wait.h>

#include "BuiltinCommand.h"
#include "CommandNotFoundException.h"
#include "FileSearcher.h"

ExternalCommand::ExternalCommand(const std::string &name) {
    this->CmdName = name;
}

CommandOutput_t ExternalCommand::executeWithoutRedirection(std::vector<std::string> &args) const {

    try {
        FileSearcher searcher;
        searcher.getPathToFile(CmdName);
    } catch (const FileSearcher::FileNotFoundException&) {
        throw CommandNotFoundException(CmdName);
    }

    std::vector<char *> execArgv;
    std::string cmdnameCopy = CmdName;
    execArgv.push_back(cmdnameCopy.data());
    for (auto &arg : args) {
        execArgv.push_back(arg.data());
    }
    execArgv.push_back(nullptr); // Null terminated

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
                // react on it
            }
        } else if (WIFSIGNALED(status)) {
            // react on it
        }
        return {stdoutOutput.str(), stderrOutput.str()};
    } else { // fork failed
        throw std::runtime_error("Failed to fork process");
    }
}
