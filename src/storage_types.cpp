//
// Created by brzan on 03.01.2023.
//

#include "../include/storage_types.hpp"

PackageQueueType PackageQueue::get_queue_type() const {
    return pqt_;
}

Package PackageQueue::pop() { // nie ma gwarancji ze ta metoda zadziala
    Package temp(-1);
    switch (pqt_) {
        case PackageQueueType::FIFO:
            temp = std::move(packages_.front());
            packages_.pop_front();
            break;
        case PackageQueueType::LIFO:
            temp = std::move(packages_.back());
            packages_.pop_back();
            break;
        default:
            throw;
    }
    return temp;
}

size_type PackageQueue::size() const {
    return int(packages_.size());
}

bool PackageQueue::empty() const {
    if (packages_.empty())
        return true;
    else
        return false;
}

void PackageQueue::push(Package &&package) {
    packages_.emplace_back(package.get_ID());
}