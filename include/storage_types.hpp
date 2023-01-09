// klasy IPackageStockpile, IPackageQueue, PackageQueue
// typ wyliczeniowy PackageQueueType
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <list>

#include "types.hpp"
#include "package.hpp"

enum class PackageQueueType {
    FIFO,
    LIFO
};


class IPackageStockpile {
protected: // nie wiem jak to rozumiec do konca
    std::list<Package> packages_;

public:
    using const_iterator = std::list<Package>::const_iterator;
    using iterator = std::list<Package>::iterator;

    virtual void push(Package &&package) = 0;

    virtual bool empty() const = 0;

    virtual size_type size() const = 0;

    iterator begin() { return packages_.begin(); }
    iterator end() { return packages_.end(); }
    const_iterator begin() const { return packages_.cbegin(); }
    const_iterator end() const { return packages_.cend(); }

    ~IPackageStockpile() = default;
};


class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() = default;
};


class PackageQueue : public IPackageQueue {
private:
    PackageQueueType pqt_;

public:
    PackageQueue(PackageQueueType pqt) : pqt_(pqt) {}

    PackageQueueType get_queue_type() const override;

    Package pop() override;

    size_type size() const override;

    bool empty() const override;

    void push(Package &&package) override;
};

#endif //NETSIM_STORAGE_TYPES_HPP
