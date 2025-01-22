#ifndef COMMANDNOTFOUNDEXCEPTION_H
#define COMMANDNOTFOUNDEXCEPTION_H
#include <stdexcept>


class CommandNotFoundException final : public std::runtime_error {
public:
    explicit CommandNotFoundException(const std::string& name)
        : std::runtime_error( name + ": command not found") {}
};


#endif //COMMANDNOTFOUNDEXCEPTION_H
