#pragma once

#include "string.h"
#include "../../common/config.h"

class Page {
public:
    bool is_dirty = false;
    char data[PAGE_SIZE]{};

    Page(int page_id);
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
