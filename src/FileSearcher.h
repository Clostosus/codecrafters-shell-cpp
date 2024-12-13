#ifndef FILESEARCHER_H
#define FILESEARCHER_H
#include <string>
#include <vector>
#include <filesystem>
#include <sys/stat.h>

class FileSearcher {
private:
    std::vector<std::string> pathFilenames;
    void addPathDirsFilesToList();
    void addDirFilenamesToList(const std::string& DirPath);
public:
    FileSearcher();
    ~FileSearcher()=default;
    std::string getPathToFile(const std::string &filename);
    void printFilenames() const;
};


#endif //FILESEARCHER_H
