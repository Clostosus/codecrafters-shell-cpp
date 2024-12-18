#ifndef FILESEARCHER_H
#define FILESEARCHER_H
#include <string>
#include <vector>
#include <filesystem>



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

    class FileNotFoundException final : public std::runtime_error{
        public:
        explicit FileNotFoundException(const std::string &message): runtime_error(message) {}
    };
};


#endif //FILESEARCHER_H
