#include "table_page.h"

void TablePage::Init() {
    SetTupleCount(0);
    SetFreeSpacePointer(PAGE_SIZE);
}

Result<void, Error> TablePage::InsertTuple(const Tuple &tuple, CatalogTable * catalog_table) {
    // find empty space and insert, update header, return error if none
    for (int i = 0; i < GetTupleCount(); i++) {
        if (GetTupleInfo(i).second == 0) {
            memcpy(GetData() + GetTupleInfo(i).first, &tuple.data, sizeof(int));
            SetTupleInfo(i, std::make_pair(GetTupleInfo(i).first, tuple.tuple_size));
        }
    }

    // No free space at end of free pointer
    if (GetFreeSpace() < tuple.tuple_size) {
        return Err(Error{ErrorType::Internal, ""});
    }

    // move to next point leaving room for the tuple
    SetFreeSpacePointer(GetFreeSpacePointer() - tuple.tuple_size);
    // write tuple
    memcpy(GetData() + GetFreeSpacePointer(), tuple.data, tuple.tuple_size);
    SetTupleInfo(GetTupleCount(), std::make_pair(GetFreeSpacePointer(), tuple.tuple_size));
    SetTupleCount(GetTupleCount() + 1);

    return Ok();
}

Result<Tuple*, Error> TablePage::GetTuple(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number || GetTupleInfo(rid.slot_number).second == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    // maybe move inside ctor
    Tuple * res = new Tuple{catalog_table};
    res->tuple_size = GetTupleInfo(rid.slot_number).second;
    memcpy(res->data, GetData() + GetTupleInfo(rid.slot_number).first, GetTupleInfo(rid.slot_number).second);

    return Ok(res);
}
