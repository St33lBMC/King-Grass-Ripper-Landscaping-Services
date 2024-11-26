#pragma once
#include <fmt/format.h>

#include <experimental/source_location>

#if defined(TEST_MODE)
	#include "catch2/catch_test_macros.hpp"
#endif

[[noreturn]] inline void
panic_internal(std::experimental::source_location s = std::experimental::source_location::current()) {
	fmt::print("\n      at {}:{}:{}", s.file_name(), s.line(), s.column());
	exit(-1);
}

#if defined(TEST_MODE)
	#define PANIC(...) FAIL("panicked: " + fmt::format(__VA_ARGS__));
#else

	#define PANIC(...)                                                                                                 \
		fmt::print("panicked: ");                                                                                      \
		__VA_OPT__(fmt::print(__VA_ARGS__);) panic_internal();
#endif

#define VERIFY(x, ...)                                                                                                 \
	if (!(x)) {                                                                                                        \
		fmt::print("Verification failed! ");                                                                           \
		PANIC(__VA_ARGS__)                                                                                             \
	}
#define VERIFY_NOT_REACHED() VERIFY(0)
