// klasy ujęte na diagramie UML w pakiecie Nodes
// typ wyliczeniowy ReceiverType
// klasy IPackageReceiver, Storehouse, ReceiverPreferences, PackageSender, Ramp, Worker
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

#include <memory>
#include <optional>
#include <map>


enum class ReceiverType {
    WORKER,
    STOREHOUSE
};


class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual const_iterator begin() const = 0;

    virtual const_iterator end() const = 0;

    virtual const_iterator cbegin() const = 0;

    virtual const_iterator cend() const = 0;

    virtual void receive_package(Package &&p) = 0;

    virtual ElementID get_id() const = 0;

    virtual ReceiverType get_receiver_type() const = 0;

    virtual ~IPackageReceiver() = default;
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver *, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {};

    void add_receiver(IPackageReceiver *r);

    void remove_receiver(IPackageReceiver *r);

    IPackageReceiver *choose_receiver();

    preferences_t &get_preferences() const { return const_cast<preferences_t &>(preferences_); };

    bool empty() const { return preferences_.empty(); };

    const_iterator cbegin() const { return preferences_.cbegin(); };

    const_iterator begin() const { return preferences_.begin(); };

    const_iterator cend() const { return preferences_.cend(); };

    const_iterator end() const { return preferences_.end(); };

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};


class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;

    PackageSender(PackageSender &&p) = default;

    PackageSender &operator=(PackageSender&& packageSender) noexcept;

    void send_package();

    const std::optional<Package> &get_sending_buffer() const {
        return buffer_;
    };

protected:
    void push_package(Package &&package) {
        if (!buffer_) {
            buffer_.emplace(std::move(package));
        };
    };

    std::optional<Package> buffer_ = std::nullopt;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di);

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const { return di_; };

    ElementID get_id() const { return id_; };
private:
    ElementID id_;
    TimeOffset di_;

    unsigned long long start_time_ = UINTMAX_MAX;
};


class Storehouse : public IPackageReceiver {
private:
    ReceiverType receiver_type_ = ReceiverType::STOREHOUSE;
    ElementID id_;
    std::unique_ptr<IPackageStockpile> iPackageStockpile_ptr_;

public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr = std::make_unique<PackageQueue>(PackageQueueType::FIFO));

    ElementID get_id() const override { return id_; }

    void receive_package(Package &&p) override;

    const_iterator cbegin() const override { return iPackageStockpile_ptr_->cbegin(); }

    const_iterator cend() const override { return iPackageStockpile_ptr_->cend(); }

    const_iterator begin() const override { return iPackageStockpile_ptr_->begin(); }

    const_iterator end() const override { return iPackageStockpile_ptr_->end(); }

    ReceiverType get_receiver_type() const override { return receiver_type_; }
};


class Worker : public IPackageReceiver, public PackageSender {
private:
    ReceiverType receiver_type_ = ReceiverType::WORKER;
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> queue_ptr_;
    Time package_processing_start_time_ = 0;
    std::optional<Package> currently_processed_package_ = std::nullopt;

public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);

    void do_work(Time time);

    TimeOffset get_processing_duration() const { return processing_duration_; }

    Time get_package_processing_start_time() const { return id_; }


    const_iterator cbegin() const override { return queue_ptr_->cbegin(); }

    const_iterator cend() const override { return queue_ptr_->cend(); }

    const_iterator begin() const override { return queue_ptr_->cbegin(); }

    const_iterator end() const override { return queue_ptr_->cend(); }

    ElementID get_id() const override { return id_; }

    ReceiverType get_receiver_type() const override { return receiver_type_; }

    IPackageQueue* get_queue() const { return queue_ptr_.get(); }

    void receive_package(Package &&p) override;

    const std::optional<Package>& get_processing_buffer() const { return currently_processed_package_; }
};


#endif //NETSIM_NODES_HPP
