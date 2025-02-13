#ifndef FILESEARCHER_H
#define FILESEARCHER_H
#include <string>
#include <vector>
#include <filesystem>

class FileSearcher {
private:
    std::vector<std::string> pathFilenames;
    std::vector<std::string> executableNames;

    static FileSearcher* instance;

    void addPathDirsFilesToList();
    void addDirFilenamesToList(const std::string& DirPath);
    void initPathExecutableNames();
    FileSearcher();
    ~FileSearcher()=default;
public:
    static FileSearcher * getInstance();
    FileSearcher(const FileSearcher&) = delete;

    std::string getPathToFile(const std::string &filename);
    [[nodiscard]] std::vector<std::string> getExecutablesFromPath() const;
    static std::string getHomedir();

    class FileNotFoundException final : public std::runtime_error{
        public:
        explicit FileNotFoundException(const std::string &message): runtime_error(message) {}
    };
};

#endif //FILESEARCHER_H
