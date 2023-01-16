//
// Created by brzan on 03.01.2023.
//

#include "../include/nodes.hpp"

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) {
    id_ = id;
    iPackageStockpile_ptr_ = std::move(ips_ptr);
}


void Storehouse::receive_package(Package &&p) {
    iPackageStockpile_ptr_->push(std::move(p)); //todo zapytac czy ta metoda ma cos wiecej robic
}