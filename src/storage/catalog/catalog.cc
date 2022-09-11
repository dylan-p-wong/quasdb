#include <algorithm>

#include "catalog.h"

Catalog::Catalog() {
    // Init from storage
}

Result<void, Error> Catalog::CreateTable(CatalogTable * table) {
    for (CatalogTable * existing_table : tables) {
        if (existing_table->name == table->name) {
            return Err(Error{ErrorType::Internal, "Table with name already exists."});
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
