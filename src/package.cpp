//
// Created by brzan on 03.01.2023.
//

#include "../include/package.hpp"


ElementID Package::BLANK_ID = -1;
std::set<ElementID> Package::assigned_IDs = {0};
std::set<ElementID> Package::freed_IDs = {};

ElementID Package::get_id() const {
    return ID_;
}

Package::~Package() {
    if (ID_ != BLANK_ID) {
        assigned_IDs.erase(ID_);
        freed_IDs.insert(ID_);
    }
}

Package::Package() {
    if (!freed_IDs.empty()) {
        ID_ = *freed_IDs.begin();
        freed_IDs.erase(ID_);
    }
    else {
        ID_ = *assigned_IDs.end();
        assigned_IDs.insert(ID_);
    }
}

Package::Package(ElementID id) {
    assigned_IDs.insert(id);
    if (freed_IDs.find(id) != freed_IDs.end()) {
        freed_IDs.erase(id);
    }
    ID_ = id;
}

Package& Package::operator=(Package &&other)  noexcept {
    ID_ = other.ID_;
    other.ID_ = BLANK_ID;
    return *this;
}

Package::Package(Package &&package) noexcept {
    ID_ = package.ID_;
    package.ID_ = BLANK_ID;
}
