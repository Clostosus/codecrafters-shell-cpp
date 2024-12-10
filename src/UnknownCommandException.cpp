#include "UnknownCommandException.h"

UnknownCommandException::UnknownCommandException(const std::string& Command)
: std::runtime_error(Command + ": command not found"){
    this->command = Command + ": command not found";
}

UnknownCommandException::~UnknownCommandException() = default;

std::string UnknownCommandException::message() const {
    return this->command;
}

const char * UnknownCommandException::what() const noexcept{
    return this->command.c_str();
}



