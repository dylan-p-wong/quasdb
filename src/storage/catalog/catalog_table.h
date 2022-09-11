#pragma once

#include <string>
#include <vector>

#include "../../common/result.h"
#include "../../common/error.h"
#include "catalog_column.h"

class CatalogTable {
public:
    std::string name;
    std::vector<CatalogColumn*> columns;
    // Result<CatalogColumn*, Error> GetColumn(std::string name);
    // Result<void, Error> GetColumnIndex(std::string name);
    // Result<void, Error> GetPrimaryKey(std::string name);
    // bool ValidateTable();
    // bool ValidateRow();
};
