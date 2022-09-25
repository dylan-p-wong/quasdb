#pragma once

#include <string>
#include <optional>
#include <memory>

#include "../../common/shared.h"
#include "../../parser/statement/create_table.h"

class CatalogColumn {
    DataType datatype;
    int column_offset; // offset in tuple
    std::string name;
    bool primary_key;
    bool nullable = true;
    std::unique_ptr<AbstractData> default_value;
    bool unique;
    bool index;
    std::optional<std::pair<std::string, std::string>> references; // table.field
    int size; // varchar only (length)
public:

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

    std::optional<std::pair<std::string, std::string>> GetReferences() {
        return references;
    }

    bool GetPrimaryKey() {
        return primary_key;
    }

    bool GetNullable() {
        return nullable;
    }

    AbstractData * GetDefaultValue() {
        return default_value == nullptr ? nullptr : default_value.get();
    }

    bool GetUnique() {
        return unique;
    }

    bool GetIndex() {
        return index;
    }
};
