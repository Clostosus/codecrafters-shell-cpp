#include "ExternalCommand.h"

#include "CommandNotFoundException.h"
#include "FileSearcher.h"

void ExternalCommand::execute(std::vector<std::string> &args) const {
    std::string pathToFile;
    try {
        // Suche den Pfad zur Datei
        FileSearcher searcher;
        pathToFile = searcher.getPathToFile(name);
    } catch (const FileSearcher::FileNotFoundException&) {
        throw CommandNotFoundException("Command not found: " + name);
    }

    try {
        // Erzeuge und führe den SubprogramExecutor aus
        SubprogramExecutor executor(pathToFile, args);
        executor.execute();
    } catch (const SubprogramExecutor::SubprogramExecutorException& e) {
        std::cerr << e.what() << std::endl;
    }
}
