#pragma once

#include <string>
#include <vector>

#include "../../common/result.h"
#include "../../common/error.h"
#include "catalog_table.h"
#include "../../common/execution_output.h"
#include "../buffer/buffer_manager.h"

class Catalog {
    // int first_table_directory_page_id = -1;
    std::vector<CatalogTable*> tables;
    BufferManager * buffer_manager;
public:
    
    Result<void, Error> CreateTable(CatalogTable * table);
    Result<void, Error> DeleteTable(const std::string & table_name);
    Result<CatalogTable*, Error> ReadTable(const std::string & table_name);
    Catalog(BufferManager * buffer_manager);

    std::vector<CatalogTable*> GetTables() {
        return tables;
    }
    BufferManager * GetCatalogBufferManager() {
        return buffer_manager;
    }
};
