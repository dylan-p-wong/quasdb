#pragma once

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "expression.h"
#include "statement.h"

class UpdateStatement : public Statement {
public:
    std::string table;
    std::unordered_map<std::string, Expression*> set;
    Expression * where;
    UpdateStatement();
};
