#pragma once

#include <atomic>
#include <array>
#include <unordered_map>
#include <list>

#include "../page/page.h"
#include "../disk/disk_manager.h"

class BufferManager {
    DiskManager * disk_manager;
    std::list<int> empty_buffer_slots_list;
    std::unordered_map<int, int> buffer_slot_hash_table;
    std::atomic<int> next_page_id = 0; // calculate by file size / PAGE_SIZE
    std::array<Page*, 64> buffer_pages;
public:
    BufferManager(DiskManager * disk_manager);
    Page * NewPage();
    Page * GetPage(int page_id);
    bool FlushPage(int page_id);
    bool EvictPage();
    bool PutPageInBuffer(Page * page);
    bool FlushAllPages();

    // for tests
    int GetNextPageId() { return next_page_id; }
};
