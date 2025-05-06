#pragma once

#include <cstdint>
#include <stdexcept>
#include "library.hpp"

#ifdef _WIN32
#define UNREACHABLE __assume(0)
#else
#define UNREACHABLE __builtin_unreachable()
#endif

[[noreturn]] inline void TODO(const char* msg) {
    throw std::runtime_error("TODO: " + std::string(msg));
}

inline int generateNextLibraryID(Library& lib) {
    uint64_t nextID = 0;
    for (auto& user : lib.allUsers()) {
        if (user.id > nextID) {
            nextID = user.id;
        }
    }
    return nextID + 1;
}
