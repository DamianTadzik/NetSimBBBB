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

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual const_iterator begin() = 0;
    virtual const_iterator end() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

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

    const_iterator begin() override { return iPackageStockpile_ptr_->begin(); }
    const_iterator end() override { return iPackageStockpile_ptr_->end(); }
    const_iterator begin() const override { return iPackageStockpile_ptr_->begin(); }
    const_iterator end() const override {return iPackageStockpile_ptr_->end(); }


};

//class Worker : public PackageSender {
//
//};



#endif //NETSIM_NODES_HPP
