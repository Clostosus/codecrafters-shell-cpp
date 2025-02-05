#include "AbstractCommand.h"

RedirectionInfo AbstractCommand::parseRedirection(std::vector<std::string> &args) {
    RedirectionInfo redirection;
    if (!args.empty()) {
        for(int i=0; i < args.size(); i++) {
            if(!redirection.required) {
                if(args.at(i) == ">" || args[i] == "1>") {
                    redirection.required = true; redirection.redirStream = STDOUT_FILENO;
                }
                else if(args.at(i) == "2>") {
                    redirection.required = true; redirection.redirStream = STDERR_FILENO;
                }
                else if(args[i] == ">>" || args[i] == "1>>") {
                    redirection.required = true; redirection.redirStream = STDOUT_FILENO; redirection.append = true;
                }
                else if(args[i] == "2>>"){redirection.required = true; redirection.redirStream = STDERR_FILENO; redirection.append = true;}

                // Delete redir Argument
                if(redirection.required){ args.erase(args.begin()+i); i--;}
            }else {
                redirection.redirectPath = args.at(i);
                args.erase(args.begin()+i); i--;
            }
        }
    }
    return redirection;
}

void AbstractCommand::performRedirection(const RedirectionInfo &info, const CommandOutput_t &output) {
    if(info.required) {
        std::fstream file;
        if(!info.append) {
            file = std::fstream(info.redirectPath, std::ios::out);
        }else {
            file = std::fstream(info.redirectPath, std::ios::app);
        }

        if(!file.is_open()) {
            return; // Or throw Exception
        }

        if(info.redirStream == STDOUT_FILENO) {
            file.write(output.stdoutOutput.data(), static_cast<long>(output.stdoutOutput.size()) );
        }else if(info.redirStream == STDERR_FILENO) {
            file.write(output.stderrOutput.data(), static_cast<long>(output.stderrOutput.size()) );
            std::cout << "Cout: " << output.stdoutOutput;
        }

        file.close();
    } else {
        std::cout << output.stdoutOutput;
        std::cerr << output.stderrOutput;
    }
}
