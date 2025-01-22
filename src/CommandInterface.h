#ifndef COMMANDINTERFACE_H
#define COMMANDINTERFACE_H
#include <string>
#include <vector>


class CommandInterface {
public:
    virtual ~CommandInterface() = default;
    virtual void execute(std::vector<std::string>& args) const = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
    [[nodiscard]] virtual std::string getDescription() const = 0;
};




#endif //COMMANDINTERFACE_H
