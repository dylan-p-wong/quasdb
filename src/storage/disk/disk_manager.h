#pragma once

#include <sys/stat.h>

#include <fstream>
#include <string>

class DiskManager {
    std::fstream db_io;
    std::string file_name;
public:
    DiskManager(std::string file_name);
    ~DiskManager();
    void WritePage(int page_id, char * page_data);
    void ReadPage(int page_id, char * page_data);
    int GetFileSize() {
        struct stat stat_buf;
        int rc = stat(file_name.c_str(), &stat_buf);
        return rc == 0 ? static_cast<int>(stat_buf.st_size) : -1;
    }
};
