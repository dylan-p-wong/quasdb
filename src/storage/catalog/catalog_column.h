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
    // std::optional<std::string> references; // not implemented yet. need binder
    // bool ValidateColumn();
    // bool ValidateValue();
    CatalogColumn(const Column & statement_column) : 
    name{statement_column.name}, 
    datatype{statement_column.data_type},
    primary_key{statement_column.primary_key},
    nullable{statement_column.nullable},
    default_value{statement_column.default_value != nullptr ? statement_column.default_value->EvaluateConstant() : std::unique_ptr<AbstractData>{}},
    unique{statement_column.unique},
    index{statement_column.index} {}
    // references{}, still need binder
};
