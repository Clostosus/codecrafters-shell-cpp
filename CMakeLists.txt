cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES}
        src/Command.cpp
        src/Command.h
        src/CommandManager.cpp
        src/CommandManager.h
        src/FileSearcher.cpp
        src/FileSearcher.h
        src/SubprogramExecutor.cpp
        src/SubprogramExecutor.h
        src/Builtins.cpp
        src/Builtins.h
        src/CommandReader.cpp
        src/CommandReader.h
        src/ArgumentsParser.cpp
        src/ArgumentsParser.h
)
