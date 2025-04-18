#pragma once

#include <vector>

#include "librarystoragetype.hpp"

class Library;

template <typename T> requires LibraryStorageType<T>
class ResultList {
    std::vector<T*> items;
    Library& lib;
    bool modified = false;

    ResultList(Library& lib, std::vector<T*>&& items) : items(items), lib(lib) {}
    
public:
    ResultList(ResultList& lib) = delete;
    ResultList(const ResultList& other) = delete;

    ResultList& operator=(const ResultList& other) = delete;
    ResultList& operator=(ResultList&& other) = delete;
    
    ~ResultList();
    
    T& operator[](int index);
    const T& operator[](int index) const;

    int size() const;

    friend class Library;
};
