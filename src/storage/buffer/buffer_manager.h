#pragma once

#include <atomic>
#include <array>
#include <unordered_map>
#include <list>

#include "../page/page.h"

class BufferManager {
public:
    std::list<int> empty_buffer_slots_list;
    std::unordered_map<int, int> buffer_slot_hash_table;
    std::atomic<int> next_page_id = 0;
    std::array<Page*, 64> buffer_pages;
    BufferManager();
    Page * NewPage();
    Page * GetPage(int page_id);
    // bool FlushPage(int page_id);
    bool UnpinPage();
    bool PinPage(Page * page);
};
