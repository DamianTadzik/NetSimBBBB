// klasy ujęte na diagramie UML w pakiecie Nodes
// typ wyliczeniowy ReceiverType
// klasy IPackageReceiver, Storehouse, ReceiverPreferences, PackageSender, Ramp, Worker
//
// Created by brzan on 03.01.2023.
//

#include "package.hpp"
#include "storage_types.hpp"

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <memory>
#include <optional>

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual const_iterator begin() const= 0;
    virtual const_iterator end() const= 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual void receive_package(Package &&p) = 0;
    virtual ElementID get_id() const = 0;
};

class Storehouse : public IPackageReceiver {
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> iPackageStockpile_ptr_;

public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr);

    ElementID get_id() const override { return id_;}

    void receive_package(Package &&p) override;

    const_iterator cbegin() const override { return iPackageStockpile_ptr_->cbegin(); }
    const_iterator cend() const override { return iPackageStockpile_ptr_->cend(); }
    const_iterator begin() const override { return iPackageStockpile_ptr_->begin(); }
    const_iterator end() const override {return iPackageStockpile_ptr_->end(); }
};

class Worker : public IPackageReceiver{
private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> queue_ptr_;
    Time package_processing_start_time_ = 0;
    std::optional<Package> currently_processed_package_;

public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), processing_duration_(pd), queue_ptr_(std::move(q)) {};

    void do_work(Time time);

    TimeOffset get_processing_duration() const{ return processing_duration_; }

    Time get_package_processing_start_time() const { return id_; }

    const_iterator cbegin() const override { return queue_ptr_->cbegin(); }

    const_iterator cend() const override { return queue_ptr_->cend(); }

    const_iterator begin() const override { return queue_ptr_->cbegin(); }

    const_iterator end() const override { return queue_ptr_->cend(); }

    ElementID get_id() const override { return id_; }

    void receive_package(Package &&p) override;
};



#endif //NETSIM_NODES_HPP
