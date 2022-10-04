#pragma once

#include <string>
#include <vector>

#include "../../common/result.h"
#include "../../common/error.h"
#include "catalog_table.h"
#include "../../common/execution_output.h"
#include "../buffer/buffer_manager.h"

class Catalog {
    std::vector<CatalogTable*> tables;
    BufferManager * buffer_manager;

    CatalogTable * table_info_table = nullptr;
    CatalogTable * column_info_table = nullptr;
public:
    
    Result<void, Error> CreateTable(CatalogTable * table);
    Result<void, Error> DeleteTable(const std::string & table_name);
    Result<CatalogTable*, Error> ReadTable(const std::string & table_name);
    Result<void, Error> InsertIntoTableInfoTable(CatalogTable * table);
    Result<void, Error> InsertInfoColumnInfoTable(CatalogTable * table);

    CatalogTable * InitTableInfoTable();
    CatalogTable * InitColumnInfoTable();

    Catalog(BufferManager * buffer_manager);

    std::vector<CatalogTable*> GetTables() {
        return tables;
    }
    BufferManager * GetCatalogBufferManager() {
        return buffer_manager;
    }

    void LoadStartingTables();
    void LoadTables();
};
