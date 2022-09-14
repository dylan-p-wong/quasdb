#include <unordered_set>

#include "catalog_table.h"

Result<CatalogColumn*, Error> CatalogTable::GetColumn(const std::string & name) {
    for (CatalogColumn * column : columns) {
        if (column->name == name) {
            return Ok(column);
        }
    }

    return Err(Error{ErrorType::Internal, "Column with name does not exists in this table."});
}

std::vector<std::pair<std::string, std::string>> CatalogTable::GetReferences() {
    std::vector<std::pair<std::string, std::string>> res;

    for (CatalogColumn * column : columns) {
        if (column->references.has_value()) {
            res.push_back(column->references.value());
        }
    }

    return res;
}

bool CatalogTable::ValidateTable() {
    std::unordered_set<std::string> column_names;

    // Duplicate column names
    for (CatalogColumn * column : columns) {
        if (column_names.find(column->name) != column_names.end()) {
            return false;
        }
        column_names.insert(column->name);
    }

    for (CatalogColumn * column : columns) {
        if (column->ValidateColumn() == false) {
            return false;
        }
    }

    return true;
}

bool CatalogTable::ValidateRow(const std::vector<std::unique_ptr<AbstractData>> & row) {

    if (row.size() != columns.size()) {
        return false;
    }

    for (int i = 0; i < row.size(); i++) {
        CatalogColumn * column = columns.at(i);
        
        if (!column->ValidateValue(row.at(i).get())) {
            return false;
        }
    }

    return true;
}
