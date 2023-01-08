#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <stdio.h>

#include "db/quas.h"


int main() {
    QuasDB db{"repl"};
    
    std::string s;

    while (std::getline(std::cin, s)) {
        auto res = db.Execute(s);

        if (res.error) {
            std::cout << res.error_message << std::endl;
        } else {
            std::cout << "SUCCESS" << std::endl;
            if (res.ToString() != "") {
                std::cout << res.ToString() << std::endl;
            }
        }
    }
}
