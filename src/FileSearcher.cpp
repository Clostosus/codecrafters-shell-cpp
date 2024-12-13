#include "FileSearcher.h"

#include <algorithm>
#include <iostream>
#include <ostream>

void FileSearcher::addDirFilenamesToList(const std::string& DirPath) {
    // This structure would distinguish a file from a directory
    struct stat sb{};

    // Looping until all the items of the directory are exhausted
    for (const auto& entry : std::filesystem::directory_iterator(DirPath)) {
        // Converting the path to const char * in the subsequent lines
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
    auto it = std::ranges::find_if(pathFilenames ,
    [&filename](const std::string& str) {
        size_t pos = str.find_last_of('/'); // Finde das letzte '/'
        std::string lastToken = (pos == std::string::npos) ? str : str.substr(pos + 1); // Extrahiere das Token
        return lastToken == filename;
    }
    );

    if (it != pathFilenames.end()) { return *it.base();}
    else { return ""; }
}

void FileSearcher::printFilenames() const {
    for (const auto& filename : this->pathFilenames) {
        std::cout << filename << std::endl;
    }
}

