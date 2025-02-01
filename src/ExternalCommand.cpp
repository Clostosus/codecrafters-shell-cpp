#include "ExternalCommand.h"

#include "CommandNotFoundException.h"
#include "FileSearcher.h"

void ExternalCommand::execute(std::vector<std::string> &args) const {
    try {
        FileSearcher searcher;
        searcher.getPathToFile(name);
    } catch (const FileSearcher::FileNotFoundException&) {
        throw CommandNotFoundException(name);
    }

    try {
        SubprogramExecutor executor(name, args);
        executor.execute();
    } catch (const SubprogramExecutor::SubprogramExecutorException& e) {
        std::cerr << e.what() << std::endl;
    }
}
