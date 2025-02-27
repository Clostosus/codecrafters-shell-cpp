#include "FileSearcher.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sys/stat.h>

FileSearcher * FileSearcher::instance = nullptr;

FileSearcher * FileSearcher::getInstance() {
    if(instance == nullptr) {
        instance = new FileSearcher();
    }
    return instance;
}


void FileSearcher::addPathDirsFilesToList() {
    const std::string BigPath = getenv("PATH");
    std::stringstream pathStream(BigPath);
    std::string SinglePath;
    while (std::getline(pathStream, SinglePath,':')) {
        this->addDirFilenamesToList(SinglePath);
    }
}

void FileSearcher::addDirFilenamesToList(const std::string& DirPath) {
    // This structure would distinguish a file from a directory
    struct stat sb{};

    // Looping until all the items of the directory are exhausted
    for (const auto& entry : std::filesystem::directory_iterator(DirPath)) {
        const std::filesystem::path& outfilename = entry.path();
        std::string outfilename_str = outfilename.string();
        const char* path = outfilename_str.c_str();

        // Testing whether the path points to a non-directory
        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            if(!pathFilenames.contains(outfilename)) {
                pathFilenames.insert(outfilename);
            }
        }
    }
}

FileSearcher::FileSearcher() {
    this->pathFilenames = std::unordered_set<std::string>();
    addPathDirsFilesToList();
    initPathExecutableNames();
}

std::string FileSearcher::getPathToFile(const std::string & filename) {

    const auto it = std::ranges::find_if(pathFilenames ,
                                         [&filename](const std::string& str) {
                                             const std::filesystem::path p(str);
                                             return p.filename() == filename;
                                         }
    );

    if (it != pathFilenames.end()) { return *it;}
    throw FileNotFoundException(filename);
}

std::vector<std::string> FileSearcher::getExecutablesFromPath() const {
    return std::vector(executableNames.begin(), executableNames.end());
}

void FileSearcher::initPathExecutableNames() {
    for (const auto& cmdpath : pathFilenames) {
        const size_t pos = cmdpath.find_last_of('/');
        if (pos != std::string::npos) {
            std::string executableName = cmdpath.substr(pos + 1);
            if(!executableNames.contains(executableName)) {
                executableNames.insert(cmdpath.substr(pos + 1));
            }
        }else {
            if(!executableNames.contains(cmdpath)) {
                executableNames.insert(cmdpath);
            }
        }
    }
}


std::string FileSearcher::getHomedir() {
    const std::string Home = getenv("HOME");
    if (Home.empty()) { throw FileNotFoundException("HOME not set"); }
    return Home;
}

