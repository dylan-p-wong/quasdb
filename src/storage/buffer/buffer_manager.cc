#include <iostream>

#include "buffer_manager.h"

BufferManager::BufferManager(DiskManager * disk_manager) : disk_manager{disk_manager} {
    next_page_id = disk_manager->GetFileSize() / PAGE_SIZE;

    for (int i = 0; i < 64; i++) {
        empty_buffer_slots_list.emplace_back(i);
    }
}

Page * BufferManager::NewPage()  {
    Page * new_page = new Page{next_page_id};
    
    PutPageInBuffer(new_page);

    ++next_page_id;
    return new_page;
}

Page * BufferManager::GetPage(int page_id) {
    std::cerr << "GETTING " << page_id << std::endl;
    if (buffer_slot_hash_table.find(page_id) == buffer_slot_hash_table.end()) {
        // read page from disk manager here
        Page * p = new Page{page_id};
        disk_manager->ReadPage(page_id, p->GetData());

        PutPageInBuffer(p);      

        return p;
    } else {
        return buffer_pages.at(buffer_slot_hash_table[page_id]);
    }
}

bool BufferManager::EvictPage() {
    if (empty_buffer_slots_list.size() > 0) {
        return false;
    }
    int slot_to_evict = rand() % 64;

    Page * to_delete = buffer_pages.at(slot_to_evict);
    buffer_slot_hash_table.erase(to_delete->GetPageId());
    empty_buffer_slots_list.emplace_back(slot_to_evict);

    FlushPage(to_delete->GetPageId());

    delete to_delete;

    return true;
}

bool BufferManager::PutPageInBuffer(Page * page) {
    if (empty_buffer_slots_list.size() == 0) {
        EvictPage();
    }

    buffer_slot_hash_table.insert(std::make_pair(page->GetPageId(), empty_buffer_slots_list.front()));
    buffer_pages.at(empty_buffer_slots_list.front()) = page;
    empty_buffer_slots_list.pop_front();

    return true;
}

bool BufferManager::FlushPage(int page_id) {
    std::cerr << "FLUSHING " << page_id << std::endl;
    if (buffer_slot_hash_table.find(page_id) != buffer_slot_hash_table.end()) {
        Page * p = buffer_pages.at(buffer_slot_hash_table[page_id]);
        disk_manager->WritePage(page_id, p->GetData());
        return true;
    }
    return false;
}

bool BufferManager::FlushAllPages() {
    for (auto & it : buffer_slot_hash_table) {
        FlushPage(it.first);
    }
    return true;
}
