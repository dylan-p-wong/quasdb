#pragma once

#include <string>
#include <optional>
#include <memory>

#include "../../common/shared.h"
#include "../../parser/statement/create_table.h"

class CatalogColumn {
public:
    std::string name;
    DataType datatype;
    bool primary_key;
    bool nullable;
    std::unique_ptr<AbstractData> default_value;
    bool unique;
    bool index;
    std::optional<std::pair<std::string, std::string>> references; // table.field
    bool ValidateColumn();
    // bool ValidateValue();
    CatalogColumn(const Column & statement_column);
};
