#pragma once

#include <memory>

#include "rid.h"
#include "../../common/shared.h"

class CatalogTable;

class Tuple {
    char * data{nullptr};
    int tuple_size;
public:
    Tuple(const char * data_page, const int offset, const int size, const CatalogTable * catalog_table);
    Tuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table);
    std::unique_ptr<AbstractData> GetValueAtColumnIndex(int index, const CatalogTable * catalog_table);
    char * GetData() const {
        return data;
    }
    int GetTupleSize() const {
        return tuple_size;
    }
};
