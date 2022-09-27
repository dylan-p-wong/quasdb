#pragma once

#include "page.h"
#include "table_page.h"
#include "../table/tuple.h"
#include "../../common/config.h"
#include "../../common/result.h"
#include "../../common/error.h"
#include "../buffer/buffer_manager.h"

// https://cs186berkeley.net/fa20/resources/static/notes/n02-DisksFiles.pdf
class DirectoryPage : public Page {
    static constexpr int SIZE_DIRECTORY_PAGE_HEADER = 16;
    static constexpr int OFFSET_PREV_PAGE_ID = 4;
    static constexpr int OFFSET_NEXT_PAGE_ID = 8;
    static constexpr int OFFSET_NUMBER_OF_DATA_PAGES = 12;
    static constexpr int SIZE_DATA_PAGE_INFO = 8;
public:
    void Init();

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
        return PAGE_SIZE / 8 - SIZE_DIRECTORY_PAGE_HEADER;
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
    int GetDataPagePageId(int i) {
        return *reinterpret_cast<int*>(GetData() + SIZE_DIRECTORY_PAGE_HEADER + SIZE_DATA_PAGE_INFO * i);
    }
    void SetDataPagePageId(int i, int page_id) {
        memcpy(GetData() + SIZE_DIRECTORY_PAGE_HEADER + SIZE_DATA_PAGE_INFO * i, &page_id, sizeof(int));
    }
    int GetDataPageFreeSpace(int i) {
        return *reinterpret_cast<int*>(GetData() + SIZE_DIRECTORY_PAGE_HEADER + SIZE_DATA_PAGE_INFO * i + 4);
    }
    void SetDataPageFreeSpace(int i, int free_space) {
        memcpy(GetData() + SIZE_DIRECTORY_PAGE_HEADER + SIZE_DATA_PAGE_INFO * i + 4, &free_space, sizeof(int));
    }
    int GetDataPageIdSlot(int page_id) {
        for (int i = 0; i < GetNumberOfDataPages(); i++) {
            if (GetDataPagePageId(i) == page_id) {
                return i;
            }
        }
        return -1;
    }

    Result<void, Error> InsertTuple(const InputTuple &tuple, BufferManager * buffer_manager, CatalogTable * catalog_table);
    Result<OutputTuple*, Error> GetTuple(const RID &rid, BufferManager * buffer_manager, CatalogTable * catalog_table);
    Result<void, Error> MarkDelete(const RID & rid, BufferManager * buffer_manager, const CatalogTable * catalog_table);
};
