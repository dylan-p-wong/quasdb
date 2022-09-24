#pragma once

#include <string>
#include <optional>
#include <memory>

#include "../../common/shared.h"
#include "../../parser/statement/create_table.h"

class CatalogColumn {
    DataType datatype;
    // offset in tuple
    int column_offset;
public:
    int size; // bytes
    std::string name;
    bool primary_key;
    bool nullable;
    std::unique_ptr<AbstractData> default_value;
    bool unique;
    bool index;
    std::optional<std::pair<std::string, std::string>> references; // table.field
    CatalogColumn(const Column & statement_column, int column_offset);

    // validation
    bool ValidateColumn();
    bool ValidateValue(AbstractData * value);

    int GetColumnOffset() {
        return column_offset;
    }
    int GetColumnSize();

    std::string GetColumnName() {
        return name;
    }

    DataType GetColumnDataType() {
        return datatype;
    }
};
