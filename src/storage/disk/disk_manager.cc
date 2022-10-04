#include <filesystem>
#include <iostream>

#include "disk_manager.h"
#include "../../common/config.h"
#include "../../common/error.h"


DiskManager::DiskManager(std::string file_name) : file_name{file_name} {
    db_io.open(file_name, std::ios::binary | std::ios::in | std::ios::out);

    if (!db_io.is_open()) {
        db_io.clear();

        // create a new file
        db_io.open(file_name, std::ios::binary | std::ios::trunc | std::ios::out | std::ios::in);
        if (!db_io.is_open()) {
            throw Error{ErrorType::Internal, "Unable to open file."};
        }
    }
}

DiskManager::~DiskManager() {
    db_io.close();
}

void DiskManager::WritePage(int page_id, char * page_data) {
    int offset = page_id * PAGE_SIZE;

    db_io.seekp(offset);
    db_io.write(page_data, PAGE_SIZE);

    if (db_io.bad()) {
        throw Error{ErrorType::Internal, "Bad disk write."};
    }

    db_io.flush();
}

void DiskManager::ReadPage(int page_id, char * page_data) {
    int offset = page_id * PAGE_SIZE;

    db_io.seekp(offset);
    db_io.read(page_data, PAGE_SIZE);

    if (db_io.bad()) {
        throw Error{ErrorType::Internal, "Bad disk read."};
    }
    int read_count = db_io.gcount();
    if (read_count < PAGE_SIZE) {
        db_io.clear();
        std::string e = "Did not read a page " + std::to_string(page_id);
        throw Error{ErrorType::Internal, e};
    }
}

void DiskManager::DeleteFile() {
    try {
        if (std::filesystem::remove(file_name)) {
            std::cerr << "file " << file_name << " deleted.\n";
        } else {
            std::cerr << "file " << file_name << " not found.\n";
        }
    } catch(const std::filesystem::filesystem_error& err) {
        std::cerr << "filesystem error: " << err.what() << '\n';
    }
}
