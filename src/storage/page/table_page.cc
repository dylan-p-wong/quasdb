#include "table_page.h"

void TablePage::Init() {
    SetTupleCount(0);
    SetFreeSpacePointer(PAGE_SIZE);
}

Result<void, Error> TablePage::InsertTuple(const Tuple &tuple, CatalogTable * catalog_table) {
    // find empty space and insert, update header, return error if none
    for (int i = 0; i < GetTupleCount(); i++) {
        if (GetTupleInfo(i).second == 0) {
            memcpy(GetData() + GetTupleInfo(i).first, tuple.GetData(), sizeof(int));
            SetTupleInfo(i, std::make_pair(GetTupleInfo(i).first, tuple.GetTupleSize()));
        }
    }

    // No free space at end of free pointer
    if (GetFreeSpace() < tuple.GetTupleSize()) {
        return Err(Error{ErrorType::Internal, ""});
    }

    // move to next point leaving room for the tuple
    SetFreeSpacePointer(GetFreeSpacePointer() - tuple.GetTupleSize());
    // write tuple
    memcpy(GetData() + GetFreeSpacePointer(), tuple.GetData(), tuple.GetTupleSize());
    SetTupleInfo(GetTupleCount(), std::make_pair(GetFreeSpacePointer(), tuple.GetTupleSize()));
    SetTupleCount(GetTupleCount() + 1);

    return Ok();
}

Result<Tuple*, Error> TablePage::GetTuple(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number || GetTupleInfo(rid.slot_number).second == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    if (GetTupleInfo(rid.slot_number).second < 0) {
        return Err(Error{ErrorType::Internal, "Tuple is deleted."});
    }

    Tuple * res = new Tuple{GetData(), GetTupleInfo(rid.slot_number).first, GetTupleInfo(rid.slot_number).second, catalog_table};
    return Ok(res);
}

std::pair<int, int> TablePage::GetTupleInfo(int i) {
    int pointer = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i);
    int size = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4);
    return std::make_pair(pointer, size);
}

void TablePage::SetTupleInfo(int i, std::pair<int, int> info) {
    memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i, &info.first, sizeof(int));
    memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4, &info.second, sizeof(int));
}

Result<void, Error> TablePage::MarkDelete(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    int tuple_size = GetTupleInfo(rid.slot_number).second;
    
    if (tuple_size == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    if (tuple_size < 0) {
        return Err(Error{ErrorType::Internal, "Tuple is already deleted."});
    }

    SetTupleInfo(rid.slot_number, std::make_pair(GetTupleInfo(rid.slot_number).first, tuple_size * -1));

    return Ok();
}
