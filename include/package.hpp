// klasa Package
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <set>
#include <stdexcept>
#include "types.hpp"


class Package {
private:
    ElementID ID_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    static ElementID BLANK_ID;

public:
    Package();

    explicit Package(ElementID id);

    Package(Package &&package) noexcept;

    Package(Package &package) = delete;

    Package &operator=(Package &&other) noexcept;

    Package &operator=(Package &other) = delete;

    ElementID get_id() const;

    ~Package();
};

#endif //NETSIM_PACKAGE_HPP
