#include "FileSearcher.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sys/stat.h>

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

        // Testing whether the path points to a non-directory or not If it does, displays path
        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            pathFilenames.push_back(outfilename);
        }
    }
}

FileSearcher::FileSearcher() {
    this->pathFilenames = std::vector<std::string>();
}

std::string FileSearcher::getPathToFile(const std::string & filename) {
    addPathDirsFilesToList();

    const auto it = std::ranges::find_if(pathFilenames ,
                                         [&filename](const std::string& str) {
                                             const std::filesystem::path p(str);
                                             return p.filename() == filename;
                                         }
    );

    if (it != pathFilenames.end()) { return *it.base();}
    else{throw FileNotFoundException(filename);}
}

void FileSearcher::printFilenames() const {
    for (const auto& filename : this->pathFilenames) {
        std::cout << filename << std::endl;
    }
}

std::string FileSearcher::getHomedir() {
    const std::string Home = getenv("HOME");
    if (Home.empty()) { throw FileNotFoundException("HOME not set"); }
    return Home;
}

