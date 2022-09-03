#pragma once

#include <string>


enum class ErrorType { Abort, Config, Internal, Lex, Parse, ReadOnly, Serialization, Value };

struct Error {
    ErrorType type;
    std::string message;

    Error(ErrorType type, std::string text) : type{type}, message{message} {}
};
