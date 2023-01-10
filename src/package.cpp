//
// Created by brzan on 03.01.2023.
//

#include "../include/package.hpp"

Package &Package::operator=(Package &&other) {
    ID_ = other.ID_;
    return *this;
}

ElementID Package::get_ID() const {
    return ID_;
}

Package::~Package() {
    freed_IDs.emplace(ID_);
}

Package::Package() {
    if (!freed_IDs.empty()) {
        ID_ = *freed_IDs.lower_bound(0);
        freed_IDs.erase(ID_);
    }
    else {
        int n = int(assigned_IDs.size());
        ID_ = *assigned_IDs.lower_bound(n) + 1;
        assigned_IDs.emplace(ID_);
    }
}
