#pragma once

#include <string>

#include "expression.h"
#include "statement.h"

class DeleteStatement : public Statement {
public:
    std::string table;
    Expression * where;
    DeleteStatement();
};
