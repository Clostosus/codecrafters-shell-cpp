#include "ExternalCommand.h"

#include "CommandNotFoundException.h"
#include "FileSearcher.h"

void ExternalCommand::execute(std::vector<std::string> &args) const {
    try {
        SubprogramExecutor executor(name, args);
        executor.execute();
    } catch (const SubprogramExecutor::SubprogramExecutorException& e) {
        std::cerr << e.what() << std::endl;
    }
}
