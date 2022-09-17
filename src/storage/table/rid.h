#pragma once

class RID {
public:
    int page_number;
    int slot_number;
    RID(int page_number, int slot_number) : page_number{page_number}, slot_number{slot_number} {}
};
