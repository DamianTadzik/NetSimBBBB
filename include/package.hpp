// klasa Package
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <set>
#include "types.hpp"


class Package { //poprawic nadawanie IDs'ow
private:
    ElementID ID_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

public:
    Package();

    Package(ElementID id) : ID_(id) {};

    Package(Package &&package) : ID_(package.ID_) {};

    Package &operator=(Package &&other);

    ElementID get_ID() const;

    ~Package();
};

#endif //NETSIM_PACKAGE_HPP
