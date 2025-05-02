#include "resultlist.hpp"
#include "library.hpp"

#include <cassert>

template <typename T> requires IsLibraryStorageType<T>
ResultList<T>::~ResultList() {
    if (modified) {
        lib.flushVector<T>();
    }
}

template <typename T> requires IsLibraryStorageType<T>
T& ResultList<T>::operator[](int index) {
    assert(index >= 0 && index < items.size());

    modified = true;
    return *items[index];
}

template <typename T> requires IsLibraryStorageType<T>
const T& ResultList<T>::operator[](int index) const {
    assert(index >= 0 && index < items.size());

    return *items[index];
}

template <typename T> requires IsLibraryStorageType<T>
int ResultList<T>::size() const {
    return items.size();
}

template <typename T> requires IsLibraryStorageType<T>
void ResultList<T>::remove(int index) {
    assert(index >= 0 && index < items.size());

    lib.remove(items[index]);
    items.erase(items.begin() + index);
    modified = true;
}

template class ResultList<InventoryItem>;
template class ResultList<User>;
template class ResultList<Librarian>;
