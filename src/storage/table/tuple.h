#pragma once

#include <memory>

#include "rid.h"
#include "../../common/shared.h"

class CatalogTable;

class Tuple {
public:
    Tuple(const CatalogTable * catalog_table);
    Tuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table);
    std::unique_ptr<AbstractData> GetValueAtColumnIndex(int index, const CatalogTable * catalog_table);
    char * GetData() {
        return data;
    }
    char * data{nullptr};
    int tuple_size;
};
