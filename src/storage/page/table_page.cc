#include <bitset>
#include "table_page.h"

void TablePage::Init() {
    SetTupleCount(0);
    SetFreeSpacePointer(PAGE_SIZE);
}

Result<void, Error> TablePage::InsertTuple(const InputTuple &tuple, CatalogTable * catalog_table) {
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

    SetTupleInfo(GetTupleCount(), TupleInfo{tuple.GetNullBitMap(), GetFreeSpacePointer(), tuple.GetTupleSize()}); 
    SetTupleCount(GetTupleCount() + 1);

    return Ok();
}

Result<OutputTuple*, Error> TablePage::GetTuple(const RID & rid, const CatalogTable * catalog_table) {
    if (GetTupleCount() < rid.slot_number || GetTupleInfo(rid.slot_number).tuple_size == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    if (GetTupleInfo(rid.slot_number).tuple_size < 0) {
        return Err(Error{ErrorType::Internal, "Tuple is deleted."});
    }

    OutputTuple * res = new OutputTuple{rid, GetData(), GetTupleInfo(rid.slot_number).null_bit_map, GetTupleInfo(rid.slot_number).offset, GetTupleInfo(rid.slot_number).tuple_size};
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
    short unsigned int sum = info.null_bit_map.to_ulong();
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

Result<void, Error> TablePage::UpdateTuple(const RID & rid, std::unordered_map<std::string, AbstractData*> set, const CatalogTable * catalog_table) {
if (GetTupleCount() < rid.slot_number) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    int tuple_size = GetTupleInfo(rid.slot_number).tuple_size;
    
    if (tuple_size == 0) {
        return Err(Error{ErrorType::Internal, "Tuple does not exists at this slot."});
    }

    OutputTuple * tuple = new OutputTuple{rid, GetData(), GetTupleInfo(rid.slot_number).null_bit_map, GetTupleInfo(rid.slot_number).offset, GetTupleInfo(rid.slot_number).tuple_size};

    for (auto& it: set) {
        tuple->Update(it.first, it.second, catalog_table);
    }

    memcpy(GetData() + GetTupleInfo(rid.slot_number).offset, tuple->GetData(), tuple->GetTupleSize());
    return Ok();
}
