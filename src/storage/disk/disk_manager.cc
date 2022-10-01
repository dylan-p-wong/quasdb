#include "disk_manager.h"
#include "../../common/config.h"
#include "../../common/error.h"


DiskManager::DiskManager(std::string file_name) : file_name{file_name} {
    db_io.open(file_name, std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out); // std::ios::trunc temporary

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

char * DiskManager::ReadPage(int page_id) {
    int offset = page_id * PAGE_SIZE;

    char page_data[4096]{};

    db_io.seekp(offset);
    db_io.read(page_data, PAGE_SIZE);

    if (db_io.bad()) {
        throw Error{ErrorType::Internal, "Bad disk read."};
    }
    int read_count = db_io.gcount();
    if (read_count < PAGE_SIZE) {
        db_io.clear();
        throw Error{ErrorType::Internal, "Did not read a page."};
    }

    return page_data;
}
