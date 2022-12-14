#pragma once

#include <string>
#include <vector>
#include <optional>

#include "expression.h"
#include "statement.h"

#include "../../common/shared.h"

struct Column {
    std::string name;
    DataType data_type;
    bool primary_key = false;
    bool nullable = true;
    Expression * default_value = nullptr;
    bool unique = false;
    bool index = false;
    std::optional<std::pair<std::string, std::string>> references; // table.field
    int size = 0; // for varchar
    Column(std::string name) : name{name} {}
};

class CreateTable : public Statement {
public:
    std::string name;
    std::vector<Column> columns;
    CreateTable(std::string name, std::vector<Column> columns) : Statement{StatementType::CreateTable}, name{name}, columns{columns} {}
};
