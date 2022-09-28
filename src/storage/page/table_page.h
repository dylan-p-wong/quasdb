#pragma once

#include "page.h"
#include "../table/rid.h"
#include "../table/tuple.h"
#include "../../common/result.h"
#include "../../common/error.h"
#include "../catalog/catalog_table.h"

// [offset, tuple_size, null_bit_map]
class TupleInfo {
public:
    std::bitset<16> null_bit_map;
    int offset;
    int tuple_size;
    TupleInfo(std::bitset<16> null_bit_map, int offset, int tuple_size) : null_bit_map{null_bit_map}, offset{offset}, tuple_size{tuple_size} {}
};

// [page id, free space pointer, tuple count, tuple1 offset, ...]
class TablePage : public Page {
    static constexpr int SIZE_TABLE_PAGE_HEADER = 12;
    static constexpr int OFFSET_FREE_SPACE_POINTER = 4;
    static constexpr int OFFSET_TUPLE_COUNT = 8;
    static constexpr int SIZE_TUPLE_INFO = 10;
public:
    void Init();

    Result<void, Error> InsertTuple(const InputTuple &tuple, CatalogTable * catalog_table);
    Result<OutputTuple*, Error> GetTuple(const RID & rid, const CatalogTable * catalog_table);
    Result<void, Error> MarkDelete(const RID & rid, const CatalogTable * catalog_table);
    Result<void, Error> UpdateTuple(const RID & rid, std::unordered_map<std::string, AbstractData*> set, const CatalogTable * catalog_table);

    int GetTupleCount() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_TUPLE_COUNT);
    }
    int GetFreeSpacePointer() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_FREE_SPACE_POINTER);
    }
    int GetFreeSpace() {
        return GetFreeSpacePointer() - SIZE_TABLE_PAGE_HEADER - (SIZE_TUPLE_INFO * GetTupleCount());
    }
    void SetTupleCount(int tuple_count) {
        memcpy(GetData() + OFFSET_TUPLE_COUNT, &tuple_count, sizeof(int));
    }
    void SetFreeSpacePointer(int free_space_pointer) {
        memcpy(GetData() + OFFSET_FREE_SPACE_POINTER, &free_space_pointer, sizeof(int));
    }

    // <pointer, tuple_size>
    TupleInfo GetTupleInfo(int i);

    // <pointer, tuple_size>
    void SetTupleInfo(int i, TupleInfo info);
};
