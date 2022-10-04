#include "db/quas.h"


int main() {
    try {
    QuasDB db{"repl"};

    std::cout << "FILESIZE " << db.GetFileSize() << std::endl;

    auto res = db.Execute("SELECT * FROM systeminfo");
    assert(res.error == false);
    std::cout << res.ToString() << std::endl;

    res = db.Execute("SELECT * FROM columninfo");
    assert(res.error == false);
    std::cout << res.ToString() << std::endl;

    // res = db.Execute("CREATE TABLE testing (x varchar(16), y float, z integer)");
    // assert(res.error == false);

    res = db.Execute("INSERT into testing values ('DOOM', 6.9, 453)");
    std::cerr << res.error_message << std::endl;
    assert(res.error == false);

    res = db.Execute("SELECT * FROM test");
    std::cerr << res.error_message << std::endl;
    std::cerr << res.ToString() << std::endl;
    assert(res.error == false);
    assert(res.rows.size() > 0);

    res = db.Execute("SELECT * FROM testing");
    std::cerr << res.error_message << std::endl;
    std::cerr << res.ToString() << std::endl;
    assert(res.error == false);
    assert(res.rows.size() > 0);

    // res = db.Execute("SELECT * FROM systeminfo");
    // assert(res.error == false);
    // std::cout << res.ToString() << std::endl;

    // res = db.Execute("SELECT * FROM columninfo");
    // assert(res.error == false);
    // std::cout << res.ToString() << std::endl;

    // res = db.Execute("SELECT * FROM systeminfo");
    // assert(res.error == false);
    // std::cout << res.ToString() << std::endl;
    } catch (Error & e) {
        std::cout << e.message << std::endl;
    }
}
