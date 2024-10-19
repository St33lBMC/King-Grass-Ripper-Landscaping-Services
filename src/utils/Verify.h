#pragma once
#include <fmt/format.h>
#include <experimental/source_location>


[[noreturn]] inline void verify_internal(std::experimental::source_location s = std::experimental::source_location::current()) {
    fmt::print("\n      at {}:{}:{}", s.file_name(), s.line(), s.column());
    exit(-1);
}

#define VERIFY(x, ...) if (!(x)) { fmt::print("Verification failed! "); __VA_OPT__(fmt::print(__VA_ARGS__);) verify_internal(); }
#define VERIFY_NOT_REACHED() VERIFY(0)