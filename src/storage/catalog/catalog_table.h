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
    Result<CatalogColumn*, Error> GetColumn(const std::string & name);
    std::vector<std::pair<std::string, std::string>> GetReferences();
    // Result<void, Error> GetColumnIndex(std::string name);
    // Result<void, Error> GetPrimaryKey(std::string name);
    bool ValidateTable();
    bool ValidateRow(const std::vector<std::unique_ptr<AbstractData>> & row); // May need to change to normal pointer
};
