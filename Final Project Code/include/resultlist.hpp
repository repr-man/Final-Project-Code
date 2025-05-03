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

    ResultList(ResultList& other) = delete;
    ResultList(const ResultList& other) = delete;
    ResultList& operator=(const ResultList& other) = delete;

    ResultList(ResultList&& other) : items(std::move(other.items)), lib(other.lib), modified(other.modified) {
        other.modified = false;
    }
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

    template <typename U> requires IsLibraryStorageType<U>
    const ResultList<T> join(T::FieldTag field, const ResultList<U>& other, U::FieldTag otherField) const {
        ResultList<T> res(lib);
        for(auto otherItem : other.items) {
            for(auto item : items) {
                if(item->matches(field, otherItem->get(otherField))) {
                    res.items.push_back(item);
                }
            }
        }
        return res;
    }

    template <typename U> requires IsLibraryStorageType<U>
    friend class ResultList;
    friend class Library;
};
