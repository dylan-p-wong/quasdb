#include <bitset>
#include "table_page.h"

void TablePage::Init() {
    SetTupleCount(0);
    SetFreeSpacePointer(PAGE_SIZE);
}

Result<void, Error> TablePage::InsertTuple(const Tuple &tuple, CatalogTable * catalog_table) {
    // find empty space and insert, update header, return error if none
    for (int i = 0; i < GetTupleCount(); i++) {
        if (GetTupleInfo(i).tuple_size == 0) {
            memcpy(GetData() + GetTupleInfo(i).offset, tuple.GetData(), sizeof(int));
            SetTupleInfo(i, TupleInfo{GetTupleInfo(i).null_bit_map, GetTupleInfo(i).offset, tuple.GetTupleSize()});
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

    // create null bit map
    std::bitset<16> null_bit_map;
    for (int i = 0; i < catalog_table->GetNumberOfColumns(); i++) {
        if (tuple.GetValueAtColumnIndex(i, catalog_table)->type == DataType::Null) {
            null_bit_map.set(i, true);
        }
    }

    SetTupleInfo(GetTupleCount(), TupleInfo{tuple.GetNullBitMap(), GetFreeSpacePointer(), tuple.GetTupleSize()});
    
    SetTupleCount(GetTupleCount() + 1);

    return Ok();
}

Result<Tuple*, Error> TablePage::GetTuple(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number || GetTupleInfo(rid.slot_number).tuple_size == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    if (GetTupleInfo(rid.slot_number).tuple_size < 0) {
        return Err(Error{ErrorType::Internal, "Tuple is deleted."});
    }

    Tuple * res = new Tuple{GetData(), GetTupleInfo(rid.slot_number).null_bit_map, GetTupleInfo(rid.slot_number).offset, GetTupleInfo(rid.slot_number).tuple_size, catalog_table};
    return Ok(res);
}

TupleInfo TablePage::GetTupleInfo(int i) {
    int pointer = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i);
    int size = *reinterpret_cast<int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4);
    unsigned short int t = *reinterpret_cast<unsigned short int*>(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 8);
    std::bitset<16> null_bit_map{t};
    return TupleInfo{null_bit_map, pointer, size};
}

void TablePage::SetTupleInfo(int i, TupleInfo info) {
    memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i, &info.offset, sizeof(int));
    memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 4, &info.tuple_size, sizeof(int));
    short unsigned int sum = 0;
    for (int i = 0; i < info.null_bit_map.size(); i++) {
        if (info.null_bit_map[i]) {
            sum += pow(2, i);
        }
    }
    memcpy(GetData() + SIZE_TABLE_PAGE_HEADER + SIZE_TUPLE_INFO * i + 8, &sum, sizeof(unsigned short int));
}

Result<void, Error> TablePage::MarkDelete(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    int tuple_size = GetTupleInfo(rid.slot_number).tuple_size;
    
    if (tuple_size == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    if (tuple_size < 0) {
        return Err(Error{ErrorType::Internal, "Tuple is already deleted."});
    }

    SetTupleInfo(rid.slot_number, TupleInfo{GetTupleInfo(rid.slot_number).null_bit_map, GetTupleInfo(rid.slot_number).offset, tuple_size * -1});

    return Ok();
}
