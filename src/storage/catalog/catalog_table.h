#pragma once

#include <string>
#include <vector>

#include "../../common/result.h"
#include "../../common/error.h"
#include "catalog_column.h"
#include "../table/tuple.h"
#include "../buffer/buffer_manager.h"
#include "../page/directory_page.h"

class CatalogTable {
public:
    int tuple_size = 0;
    // For pinning first page on initiliazation
    int first_data_page_directory_page_id = -1;
    std::string name;
    std::vector<CatalogColumn*> columns;
    Result<CatalogColumn*, Error> GetColumn(const std::string & name);
    std::vector<std::pair<std::string, std::string>> GetReferences();
    // Result<void, Error> GetColumnIndex(std::string name);
    // Result<void, Error> GetPrimaryKey(std::string name);
    bool ValidateTable();
    bool ValidateRow(const std::vector<std::unique_ptr<AbstractData>> & row); // May need to change to normal pointer

    int GetNumberOfColumns() const {
        return columns.size();
    }
    int GetLengthOfTuple() const {
        return tuple_size;
    }
    CatalogColumn * GetColumn(int index) const {
        return columns.at(index);
    }

    Result<void, Error> InsertTuple(const Tuple &tuple, BufferManager * buffer_manager);

    CatalogTable();
};
