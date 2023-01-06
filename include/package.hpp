// klasa Package
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP


#include "types.hpp"

class Package {
private:
    ElementID ID_;
    // static set<ElementID> assigned_IDs;

public:
    Package() = default;

    Package(ElementID id) : ID_(id) {};

    Package(Package &&package) : ID_(package.ID_) {};

    Package &operator=(Package &&other);

    ElementID get_ID() const;

    ~Package() = default;
};

#endif //NETSIM_PACKAGE_HPP
