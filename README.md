# QuasDB

QuasDB is a learning project SQL database. Currently it implements a subset of SQL features with more planning to be added in the future.

## Features

QuasDB currently runs a WIP and is not thread safe at the time being. Addionally transactions are not supported at the moment but plan to be added in the future. Here is a summary of the availble SQL features with more info being available [here](https://github.com/dylan-p-wong/quasdb/docs).

Statements
- CREATE TABLE
- DROP TABLE
- SELECT
- INSERT
- UPDATE
- DELETE

Other
- Joins
- Order By
- Limit
- Projections
- Filters

## Usage

Below is a sample CMakeLists.txt for including QuasDB in a project.
```
cmake_minimum_required(VERSION 3.14)
project(repl)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  quasdb
  GIT_REPOSITORY https://github.com/dylan-p-wong/quasdb.git
)

FetchContent_MakeAvailable(quasdb)

add_executable(repl main.cc)

target_link_libraries(repl
  PRIVATE
    quas
)

```

## References

