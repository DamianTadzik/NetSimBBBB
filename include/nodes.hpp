// klasy ujęte na diagramie UML w pakiecie Nodes
// typ wyliczeniowy ReceiverType
// klasy IPackageReceiver, Storehouse, ReceiverPreferences, PackageSender, Ramp, Worker
//
// Created by brzan on 03.01.2023.
//

#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <memory>
#include <optional>
#include <map>


class IPackageReceiver {
public:
    using const_iterator_ips = IPackageStockpile::const_iterator;

    virtual const_iterator_ips begin() const= 0;
    virtual const_iterator_ips end() const= 0;
    virtual const_iterator_ips cbegin() const = 0;
    virtual const_iterator_ips cend() const = 0;

    virtual void receive_package(Package &&p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;
};


class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)){};

    void add_receiver (IPackageReceiver* r);
    void remove_receiver (IPackageReceiver* r);
    IPackageReceiver* choose_receiver();

    preferences_t& get_preferences() const {return const_cast<preferences_t&>(preferences_);};
    bool empty () const {return preferences_.empty();};

    const_iterator cbegin() const {return preferences_.cbegin();};
    const_iterator begin() const {return preferences_.begin();};
    const_iterator cend() const { return preferences_.cend();};
    const_iterator end() const { return preferences_.end();};

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;

};


class PackageSender: public ReceiverPreferences{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender(PackageSender&& p) = default;

    void send_package();

    std::optional<Package>& get_sending_buffer(){
        return buffer_;
    };

protected:
    void push_package(Package&& package){
        if (!buffer_){
            buffer_.emplace(std::move(package));
        };
    };

    std::optional<Package> buffer_ = std::nullopt ;
};


class Storehouse : public IPackageReceiver {
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> iPackageStockpile_ptr_;

public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr);

    ElementID get_id() const override { return id_;}

    void receive_package(Package &&p) override;

    const_iterator_ips cbegin() const override { return iPackageStockpile_ptr_->cbegin(); }
    const_iterator_ips cend() const override { return iPackageStockpile_ptr_->cend(); }
    const_iterator_ips begin() const override { return iPackageStockpile_ptr_->begin(); }
    const_iterator_ips end() const override {return iPackageStockpile_ptr_->end(); }
};


class Worker : public IPackageReceiver, public PackageSender{
private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> queue_ptr_;
    Time package_processing_start_time_ = 0;
    std::optional<Package> currently_processed_package_ = std::nullopt;

public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);

    void do_work(Time time);

    TimeOffset get_processing_duration() const{ return processing_duration_; }

    Time get_package_processing_start_time() const { return id_; }

    //todo Zapytac o te iteratory
    const_iterator_ips cbegin() const override { return queue_ptr_->cbegin(); }

    const_iterator_ips cend() const override { return queue_ptr_->cend(); }

    const_iterator_ips begin() const override { return queue_ptr_->cbegin(); }

    const_iterator_ips end() const override { return queue_ptr_->cend(); }

    ElementID get_id() const override { return id_; }

    void receive_package(Package &&p) override;
};


#endif //NETSIM_NODES_HPP
