#pragma once

#include "string.h"
#include "../../common/config.h"

class Page {
    bool is_dirty = false;
    char data[PAGE_SIZE]{};
public:
    Page(int page_id);
    Page(char * page_data);
    char * GetData() {
        return data;
    }
    int GetPageId() {
        return *reinterpret_cast<int*>(GetData());
    }
    void SetPageId(int page_id) {
        memcpy(GetData(), &page_id, sizeof(int));
    }
};
