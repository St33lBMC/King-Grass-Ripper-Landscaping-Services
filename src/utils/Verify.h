#pragma once
#include <fmt/format.h>

#include <experimental/source_location>

[[noreturn]] inline void
panic_internal(std::experimental::source_location s = std::experimental::source_location::current()) {
	fmt::print("\n      at {}:{}:{}", s.file_name(), s.line(), s.column());
	exit(-1);
}

#define PANIC(...)                                                                                                     \
	fmt::print("panicked: ");                                                                                          \
	__VA_OPT__(fmt::print(__VA_ARGS__);) panic_internal();
#define VERIFY(x, ...)                                                                                                 \
	if (!(x)) {                                                                                                        \
		fmt::print("Verification failed! ");                                                                           \
		PANIC(__VA_ARGS__)                                                                                             \
	}
#define VERIFY_NOT_REACHED() VERIFY(0)
