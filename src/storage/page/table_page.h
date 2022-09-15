#pragma once

#include "page.h"
// #include "../table/tuple.h"
#include "../../common/result.h"
#include "../../common/error.h"
// #include "../table/rid.h"

class TablePage : public Page {
    // [page id, free space pointer, tuple count, tuple1 offset, tuple1 size]
public:
    static constexpr int SIZE_TABLE_PAGE_HEADER = 16;
    static constexpr int OFFSET_FREE_SPACE_POINTER = 4;
    static constexpr int OFFSET_TUPLE_COUNT = 8;
    static constexpr int OFFSET_TUPLE_SIZE = 12;

    // Result<void, Error> InsertTuple(const Tuple &tuple);
    // Result<void, Error> DeleteTuple(const RID & rid);
    TablePage(int page_id) : Page{page_id} {}
    // Result<Tuple*, Error> GetTuple(const RID & rid);

    int GetTupleCount() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_TUPLE_COUNT);
    }
    int GetFreeSpacePointer() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_FREE_SPACE_POINTER);
    }
    int GetTupleSize() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_TUPLE_SIZE);
    }
    void SetTupleCount(int tuple_count) {
        memcpy(GetData() + OFFSET_TUPLE_COUNT, &tuple_count, sizeof(int));
    }
    void SetFreeSpacePointer(int free_space_pointer) {
        memcpy(GetData() + OFFSET_FREE_SPACE_POINTER, &free_space_pointer, sizeof(int));
    }
    void SetTupleSize(int tuple_size) {
        memcpy(GetData() + OFFSET_TUPLE_SIZE, &tuple_size, sizeof(int));
    }
};
