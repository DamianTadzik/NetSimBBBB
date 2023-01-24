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
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package &&package) = 0;

    virtual bool empty() const = 0;

    virtual size_type size() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual ~IPackageStockpile() = default;
};


class IPackageQueue : public IPackageStockpile {
public:
    virtual Package pop() = 0;

    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};


class PackageQueue : public IPackageQueue {
private:
    std::list<Package> packages_;
    PackageQueueType packageQueueType_;

public:
    PackageQueue(PackageQueueType packageQueueType) : packageQueueType_(packageQueueType) {}

    PackageQueueType get_queue_type() const override;

    Package pop() override;

    size_type size() const override;

    bool empty() const override;

    void push(Package &&package) override;

    const_iterator cbegin() const override;

    const_iterator cend() const override;

    const_iterator begin() const override;

    const_iterator end() const override;
};

#endif //NETSIM_STORAGE_TYPES_HPP
