#include "buffer_manager.h"

BufferManager::BufferManager() {
    for (int i = 0; i < 64; i++) {
        empty_buffer_slots_list.emplace_back(i);
    }
}

Page * BufferManager::NewPage()  {
    Page * new_page = new Page{next_page_id};
    
    PinPage(new_page);

    ++next_page_id;
    return new_page;
}

Page * BufferManager::GetPage(int page_id) {
    if (buffer_slot_hash_table.find(page_id) == buffer_slot_hash_table.end()) {
        // go to disk but for now
        return nullptr;
    } else {
        return buffer_pages.at(buffer_slot_hash_table[page_id]);
    }
}

bool BufferManager::UnpinPage() {
    if (empty_buffer_slots_list.size() > 0) {
        return false;
    }
    // int slot_to_evict = rand() % 64;
    int slot_to_evict = 0;

    Page * to_delete = buffer_pages.at(slot_to_evict);
    buffer_slot_hash_table.erase(to_delete->GetPageId());
    empty_buffer_slots_list.emplace_back(slot_to_evict);

    delete to_delete;

    return true;
}

bool BufferManager::PinPage(Page * page) {
    if (empty_buffer_slots_list.size() == 0) {
        UnpinPage();
    }

    buffer_slot_hash_table.insert(std::make_pair(page->GetPageId(), empty_buffer_slots_list.front()));
    buffer_pages.at(empty_buffer_slots_list.front()) = page;
    empty_buffer_slots_list.pop_front();

    return true;
}
