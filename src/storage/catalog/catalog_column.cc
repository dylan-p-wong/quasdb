#include "catalog_column.h"

CatalogColumn::CatalogColumn(const Column &statement_column, int column_offset) : name{statement_column.name},
                                                               datatype{statement_column.data_type},
                                                               primary_key{statement_column.primary_key},
                                                               nullable{statement_column.nullable},
                                                               default_value{statement_column.default_value != nullptr ? statement_column.default_value->EvaluateConstant() : std::unique_ptr<AbstractData>{}},
                                                               unique{statement_column.unique},
                                                               index{statement_column.index},
                                                               references{statement_column.references},
                                                               column_offset{column_offset},
                                                               size{statement_column.size} {}

bool CatalogColumn::ValidateColumn() {
    if (primary_key == true && (nullable == true || unique == false)) {
        return false;
    }
    if (datatype == DataType::Null) {
        return false;
    }
    return true;
}

bool CatalogColumn::ValidateValue(AbstractData * value) {
    // wrong datatype
    if (datatype != value->type && !(value->type == DataType::Null && nullable)) {
        return false;
    }

    if (value->type == DataType::Varchar && dynamic_cast<Data<std::string>*>(value)->value.size() > GetColumnSize()) {
        return false;
    }
    // unique still need
    // references still need
    return true;
}

int CatalogColumn::GetColumnSize() {
    if (datatype == DataType::Boolean) {
        return 1;
    } else if (datatype == DataType::Float) {
        return 8;
    } else if (datatype == DataType::Integer) {
        return 4;
    } else if (datatype == DataType::Varchar) {
        return size;
    }
    throw Error{ErrorType::Internal, "Invalid datatype."};
}
