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
    int tuple_size = 0;
    int first_data_page_directory_page_id = -1; // For pinning first page on initiliazation
    std::string name;
    std::vector<CatalogColumn*> columns;
public:
    Result<CatalogColumn*, Error> GetColumn(const std::string & name);
    std::vector<std::pair<std::string, std::string>> GetReferences();
    bool ValidateTable();
    bool ValidateRow(const std::vector<std::unique_ptr<AbstractData>> & row);
    bool ValidateSet(std::unordered_map<std::string, AbstractData*> set);

    std::vector<CatalogColumn*> GetColumns() {
        return columns;
    }

    std::string GetTableName() {
        return name;
    }
    int GetNumberOfColumns() const {
        return columns.size();
    }
    int GetLengthOfTuple() const {
        return tuple_size;
    }
    CatalogColumn * GetColumn(int index) const {
        return columns.at(index);
    }
    int GetColumnIndexByName(std::string name) const {
        for (int i = 0; i < columns.size(); i++) {
            if (columns.at(i)->GetColumnName() == name) {
                return i;
            }
        }
        return -1;
    }

    Result<void, Error> InsertTuple(const InputTuple &tuple, BufferManager * buffer_manager);
    Result<OutputTuple*, Error> GetTuple(const RID & rid, BufferManager * buffer_manager);
    std::vector<OutputTuple*> GetTuples(BufferManager * buffer_manager);
    Result<void, Error> MarkDelete(const RID & rid, BufferManager * buffer_manager);
    Result<void, Error> UpdateTuple(const RID & rid, std::unordered_map<std::string, AbstractData*> set, BufferManager * buffer_manager);

    CatalogTable(std::string name, std::vector<CatalogColumn*> columns, int tuple_size);
};
