#pragma once

#include <stdexcept>

#ifdef _WIN32
#define UNREACHABLE __assume(0)
#else
#define UNREACHABLE __builtin_unreachable()
#endif

[[noreturn]] inline void TODO(const char* msg) {
    throw std::runtime_error("TODO: " + std::string(msg));
}
