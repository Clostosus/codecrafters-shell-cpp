#ifndef UNKNOWNCOMMANDEXCEPTION_H
#define UNKNOWNCOMMANDEXCEPTION_H
#include <stdexcept>
#include <string>


class UnknownCommandException : public std::runtime_error {
    private:
    std::string command;
    public:
    explicit UnknownCommandException(const std::string& Command);
    ~UnknownCommandException() override;
    [[nodiscard]] std::string message() const;
    [[nodiscard]] const char * what() const noexcept override;
};


#endif //UNKNOWNCOMMANDEXCEPTION_H
