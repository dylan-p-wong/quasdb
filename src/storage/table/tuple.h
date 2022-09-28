#pragma once

#include <memory>
#include <bitset>
#include <vector>

#include "rid.h"
#include "../../common/shared.h"

class CatalogTable;

class Tuple {
protected:
    std::bitset<16> null_bit_map;
    char * data{nullptr};
    int tuple_size;
public:
    Tuple(std::bitset<16> null_bit_map, char * data, int tuple_size);
    Tuple(char * data, int tuple_size);
    int GetTupleSize() const {
        return tuple_size;
    }
    char * GetData() const {
        return data;
    }
    void Update(std::string column, AbstractData * data, const CatalogTable * catalog_table);
    std::bitset<16> GetNullBitMap() const {
        return null_bit_map;
    }
    std::vector<AbstractData*> GetAsValues(const CatalogTable * catalog_table);
    std::unique_ptr<AbstractData> GetValueAtColumnIndex(int index, const CatalogTable * catalog_table) const;
};

class InputTuple : public Tuple {
public:
    InputTuple(const std::vector<std::unique_ptr<AbstractData>> & values, const CatalogTable * catalog_table);
};

class OutputTuple : public Tuple {
    RID rid;
public:
    OutputTuple(RID rid, const char * data_page, std::bitset<16> null_bit_map, const int offset, const int size);
    RID GetRID() {
        return rid;
    }
};
