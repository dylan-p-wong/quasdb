#include "page.h"

Page::Page(int page_id) {
    SetPageId(page_id);
}

Page::Page(char * page_data) {
    memcpy(data, page_data, PAGE_SIZE);
}
