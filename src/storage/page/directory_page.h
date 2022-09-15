#pragma once

#include "page.h"
#include "../../common/config.h"

class DirectoryPage : public Page {
// https://cs186berkeley.net/fa20/resources/static/notes/n02-DisksFiles.pdf
public:
    static constexpr int SIZE_TABLE_PAGE_HEADER = 16;
    static constexpr int OFFSET_PREV_PAGE_ID = 4;
    static constexpr int OFFSET_NEXT_PAGE_ID = 8;
    static constexpr int OFFSET_NUMBER_OF_DATA_PAGES = 12;
    static constexpr int SIZE_DATA_PAGE_INFO = 8;

    DirectoryPage(int page_id) : Page{page_id} {}

    int GetPreviousDirectoryPageId() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_PREV_PAGE_ID);
    }
    int GetNextDirectoryPageId() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_NEXT_PAGE_ID);
    }
    int GetNumberOfDataPages() {
        return *reinterpret_cast<int*>(GetData() + OFFSET_NUMBER_OF_DATA_PAGES);
    }
    int GetMaxNumberOfDataPages() {
        return PAGE_SIZE / 8 - SIZE_TABLE_PAGE_HEADER;
    }
    void SetPreviousDirectoryPageId(int page_id) {
        memcpy(GetData() + OFFSET_PREV_PAGE_ID, &page_id, sizeof(int));
    }
    void SetNextDirectoryPageId(int page_id) {
        memcpy(GetData() + OFFSET_NEXT_PAGE_ID, &page_id, sizeof(int));
    }
    void SetNumberOfDataPages(int number_of_data_pages) {
        memcpy(GetData() + OFFSET_NUMBER_OF_DATA_PAGES, &number_of_data_pages, sizeof(int));
    }
};
