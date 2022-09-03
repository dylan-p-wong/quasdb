#pragma once

#include <string>

#include "statement.h"

class DropTable : public Statement {
public:
    std::string name;
    DropTable(std::string name) : Statement{StatementType::DropTable}, name{name} {}
};
