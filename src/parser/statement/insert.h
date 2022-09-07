#pragma once

#include <string>
#include <vector>

#include "expression.h"
#include "statement.h"

class InsertStatement : public Statement {
public:
    std::string table;
    std::vector<std::string> columns;
    std::vector<std::vector<Expression*>> values;
    InsertStatement();
};
