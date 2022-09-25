#pragma once

#include <memory>
#include <bitset>

#include "rid.h"
#include "../../common/shared.h"

class CatalogTable;

class Tuple {
    std::bitset<16> null_bit_map;
    char * data{nullptr};
    int tuple_size;
public:
    Tuple(const char * data_page, std::bitset<16> null_bit_map, const int offset, const int size, const CatalogTable * catalog_table);
    Tuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table);
    std::unique_ptr<AbstractData> GetValueAtColumnIndex(int index, const CatalogTable * catalog_table) const;
    char * GetData() const {
        return data;
    }
    int GetTupleSize() const {
        return tuple_size;
    }
    std::bitset<16> GetNullBitMap() const {
        return null_bit_map;
    }
};
