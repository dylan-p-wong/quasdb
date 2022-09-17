#pragma once

#include "page.h"
#include "../table/rid.h"
#include "../table/tuple.h"
#include "../../common/result.h"
#include "../../common/error.h"
#include "../catalog/catalog_table.h"

// [page id, free space pointer, tuple count, tuple1 offset, ...]
class TablePage : public Page {
    static constexpr int SIZE_TABLE_PAGE_HEADER = 12;
    static constexpr int OFFSET_FREE_SPACE_POINTER = 4;
    static constexpr int OFFSET_TUPLE_COUNT = 8;
    static constexpr int SIZE_TUPLE_INFO = 8;
public:
    void Init();

    Result<void, Error> InsertTuple(const Tuple &tuple, CatalogTable * catalog_table);
    // Result<void, Error> DeleteTuple(const RID & rid);
    Result<Tuple*, Error> GetTuple(const RID & rid, const CatalogTable * catalog_table);

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
    std::pair<int, int> GetTupleInfo(int i) {
        int pointer = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i);
        int size = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4);
        return std::make_pair(pointer, size);
    }

    // <pointer, tuple_size>
    void SetTupleInfo(int i, std::pair<int, int> info) {
        memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i, &info.first, sizeof(int));
        memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4, &info.second, sizeof(int));
    }
};
