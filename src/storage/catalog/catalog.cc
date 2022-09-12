#include <algorithm>

#include "catalog.h"

Catalog::Catalog() {
    // Init from storage
}

Result<void, Error> Catalog::CreateTable(CatalogTable * table) {
    // Validate table
    if (table->ValidateTable() == false) {
        return Err(Error{ErrorType::Internal, "Bad table."});
    }

    // Duplicate table name
    for (CatalogTable * existing_table : tables) {
        if (existing_table->name == table->name) {
            return Err(Error{ErrorType::Internal, "Table with name already exists."});
        }
    }

    // Bad table column references
    for (std::pair<std::string, std::string> reference : table->GetReferences()) {
        bool found = false;

        for (CatalogTable * existing_table : tables) {
            if (existing_table->name == reference.first) {
                for (CatalogColumn * column : existing_table->columns) {
                    if (column->name == reference.second) {
                        found = true;
                    }
                }
            }
        }

        if (!found) {
            return Err(Error{ErrorType::Internal, "Bad table column reference."});
        }
    }

    tables.push_back(table);

    return Ok();
}

Result<void, Error> Catalog::DeleteTable(const std::string & table_name) {
    for (int i = 0; i < tables.size(); i++) {
        CatalogTable * existing_table = tables.at(i);

        if (existing_table->name == table_name) {
            tables.erase(tables.begin() + i);
            return Ok();
        }
    }

    return Err(Error{ErrorType::Internal, "Table with name not found."});
}

Result<CatalogTable*, Error> Catalog::ReadTable(const std::string & table_name) {
    for (CatalogTable * existing_table : tables) {
        if (existing_table->name == table_name) {
            return Ok(existing_table);
        }
    }

    return Err(Error{ErrorType::Internal, "Table with name not found."});
}