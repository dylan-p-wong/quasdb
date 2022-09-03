#pragma once

#include <string>
#include <vector>

#include "expression.h"
#include "statement.h"

enum class DataType { Boolean, Integer, Float, Varchar };

struct Column {
    std::string name;
    DataType data_type;
    bool primary_key;
    bool nullable;
    Expression * default_value;
    bool unique;
    bool index;
    std::string references;
};

class CreateTable : public Statement {
public:
    std::string name;
    std::vector<Column> columns;
    CreateTable(std::string name, std::vector<Column> columns) : Statement{StatementType::CreateTable}, name{name}, columns{columns} {}
};
