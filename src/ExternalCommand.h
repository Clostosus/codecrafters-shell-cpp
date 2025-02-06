#ifndef EXTERNALCOMMAND_H
#define EXTERNALCOMMAND_H
#include "AbstractCommand.h"
#include "SubprogramExecutor.h"
#include "CommandOutput.h"


class ExternalCommand : public AbstractCommand {
protected:
    std::string CmdName;

    CommandOutput_t executeWithoutRedirection(std::vector<std::string> &args) const override;
public:
    explicit ExternalCommand(const std::string &name);

    [[nodiscard]] std::string getName() const override { return CmdName; }
};



#endif //EXTERNALCOMMAND_H
