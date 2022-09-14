#include "catalog_column.h"

CatalogColumn::CatalogColumn(const Column &statement_column) : name{statement_column.name},
                                                               datatype{statement_column.data_type},
                                                               primary_key{statement_column.primary_key},
                                                               nullable{statement_column.nullable},
                                                               default_value{statement_column.default_value != nullptr ? statement_column.default_value->EvaluateConstant() : std::unique_ptr<AbstractData>{}},
                                                               unique{statement_column.unique},
                                                               index{statement_column.index},
                                                               references{statement_column.references} {}

bool CatalogColumn::ValidateColumn() {
    if (primary_key == true && (nullable == true || unique == false)) {
        return false;
    }

    return true;
}

bool CatalogColumn::ValidateValue(AbstractData * value) {
    return true;
}
