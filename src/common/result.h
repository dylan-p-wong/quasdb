#pragma once

#include "error.h"

template <typename T>
struct Result {
    T value;
    Error * error = nullptr;
public:
    Result(T value) : value{value} {}
    Result(T value, Error * error) : value{value}, error{error} {}
};
