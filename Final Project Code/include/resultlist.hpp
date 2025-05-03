#pragma once

#include <vector>

#include "librarystoragetype.hpp"

class Library;

template <typename T> requires IsLibraryStorageType<T>
class ResultList {
    std::vector<T*> items;
    Library& lib;
    bool modified = false;

    ResultList(Library& lib, std::vector<T*>&& items) : items(items), lib(lib) {}
    
public:
    ResultList(Library& lib) : lib(lib) {}
    ResultList(ResultList& lib) = delete;
    ResultList(const ResultList& other) = delete;

    ResultList& operator=(const ResultList& other) = delete;
    ResultList& operator=(ResultList&& other) {
        items = std::move(other.items);
        modified = other.modified;
        other.modified = false;
        return *this;
    }
    
    ~ResultList();
    
    T& operator[](int index);
    const T& operator[](int index) const;

    int size() const;

    void remove(int index);

    friend class Library;
};
