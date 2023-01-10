//
// Created by brzan on 03.01.2023.
//

#include "../include/package.hpp"

Package &Package::operator=(Package &&other) {
    ID_ = other.ID_;
    other.ID_ = BLANK_ID;
    return *this;
}

ElementID Package::get_id() const {
    return ID_;
}

Package::~Package() {
    assigned_IDs.erase(ID_);
    freed_IDs.insert(ID_);
}

Package::Package() {
    BLANK_ID = -1;
    if (!freed_IDs.empty()) {
        ID_ = *freed_IDs.begin();
        freed_IDs.erase(ID_);
    }
    else {
        ID_ = *assigned_IDs.end() + 1;
        assigned_IDs.insert(ID_);
    }
}

Package::Package(ElementID id) {
    assigned_IDs.insert(id);
    ID_ = id;
}

Package::Package(Package &&package) {
    ID_ = package.ID_;
    package.ID_ = BLANK_ID;
}
