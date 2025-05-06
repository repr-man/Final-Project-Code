#include "resultlist.hpp"
#include "library.hpp"

#include <cassert>
#include <string>

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

    if constexpr (std::is_same_v<T, InventoryItem>) {
        if (items[index]->borrowerID != -1) {
            auto borrower = lib.search(
                {User::ID},
                {std::to_string(items[index]->borrowerID)}
            );
            assert(borrower.size() == 1);
            borrower[0].numCheckedOut -= 1;
        }
        for(HistoryItem& hist : lib.allHistory()) {
            if(hist.name == items[index]->name) {
                lib.remove(&hist);
            }
        }
    } else if constexpr (std::is_same_v<T, User>) {
        for(auto& inv : lib.allInventory()) {
            if(inv.borrowerID == items[index]->id) {
                inv.borrowerID = -1;
            }
        }
        for(auto& hist : lib.allHistory()) {
            if(hist.userID == items[index]->id) {
                lib.remove(&hist);
            }
        }
        for(auto& librarian : lib.allLibrarians()) {
            if(librarian.id == items[index]->id) {
                lib.remove(&librarian);
            }
        }
    } else if constexpr (std::is_same_v<T, Librarian>) {
        for(auto& user : lib.allUsers()) {
            if(user.id == items[index]->id) {
                lib.remove(&user);
            }
        }
    }

    lib.remove(items[index]);
    items.erase(items.begin() + index);
    
    modified = true;
}

template class ResultList<InventoryItem>;
template class ResultList<User>;
template class ResultList<Librarian>;
template class ResultList<HistoryItem>;
