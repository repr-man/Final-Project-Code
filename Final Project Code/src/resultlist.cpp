#include "resultlist.hpp"
#include "library.hpp"

#include <cassert>

template <typename T> requires LibraryStorageType<T>
ResultList<T>::~ResultList() {
    if (modified) {
        lib.flushVector<T>();
    }
}

template <typename T> requires LibraryStorageType<T>
T& ResultList<T>::operator[](int index) {
    assert(index >= 0 && index < items.size());

    modified = true;
    return *items[index];
}

template <typename T> requires LibraryStorageType<T>
const T& ResultList<T>::operator[](int index) const {
    assert(index >= 0 && index < items.size());

    return *items[index];
}

template <typename T> requires LibraryStorageType<T>
int ResultList<T>::size() const {
    return items.size();
}

template <typename T> requires LibraryStorageType<T>
void ResultList<T>::remove(int index) {
    assert(index >= 0 && index < items.size());

    lib.remove<T>(items[index]);
    items.erase(items.begin() + index);
    modified = true;
}

template class ResultList<InventoryItem>;
template class ResultList<User>;
template class ResultList<HistoryItem>;
template class ResultList<Librarian>;
