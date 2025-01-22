#ifndef EXTERNALCOMMAND_H
#define EXTERNALCOMMAND_H
#include "CommandInterface.h"
#include "SubprogramExecutor.h"


class ExternalCommand final : public CommandInterface {
protected:
    std::string name;
    std::string description;
public:
    explicit ExternalCommand(const std::string& name, const std::string& description = "")
        : name(name), description(description) {}

    void execute(std::vector<std::string>& args) const override;

    [[nodiscard]] std::string getName() const override { return name; }
    [[nodiscard]] std::string getDescription() const override { return description; }
};



#endif //EXTERNALCOMMAND_H
