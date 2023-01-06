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
