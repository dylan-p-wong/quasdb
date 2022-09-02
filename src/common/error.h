#pragma once

#include <string>

struct Error {
    enum ErrorType { Abort, Config, Internal, Parse, ReadOnly, Serialization, Value };

    ErrorType type;
    std::string message;

    Error(ErrorType kind, std::string text);
};
