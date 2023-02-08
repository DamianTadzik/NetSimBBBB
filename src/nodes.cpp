//
// Created by brzan on 03.01.2023.
//

#include "../include/nodes.hpp"

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr) {
    id_ = id;
    iPackageStockpile_ptr_ = std::move(ips_ptr);
}


void Storehouse::receive_package(Package &&p) {
    iPackageStockpile_ptr_->push(std::move(p));
}


void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    preferences_.emplace(r, 1);
    for (auto i = preferences_.begin(); i != preferences_.end(); i++) {
        i->second = 1 / (double) (preferences_.size());
    }
}


IPackageReceiver *ReceiverPreferences::choose_receiver() {
    double dys = probability_generator();
    double suma = 0;
    for (auto i = preferences_.begin(); i != preferences_.end(); i++) {
        suma += i->second;
        if (suma > dys) {
            return i->first;
        }
    }
    return nullptr;
}


void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    auto temp = preferences_.find(r);
    preferences_.erase(temp);
//    preferences_.erase(r);
    if (preferences_.size() != 0) {
        for (auto i = preferences_.begin(); i != preferences_.end(); i++) {
            i->second = 1 / (double) (preferences_.size());
        }
    }
}


PackageSender& PackageSender::operator=(PackageSender &&packageSender)  noexcept {
    buffer_ = std::move(packageSender.buffer_);
    receiver_preferences_ = std::move(packageSender.receiver_preferences_);
    return *this;
}


void PackageSender::send_package() {
    if (buffer_) {
        receiver_preferences_.choose_receiver()->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}


Ramp::Ramp(ElementID id, TimeOffset di) {
    id_ = id;
    di_ = di;
}

void Ramp::deliver_goods(Time t) {
    if (start_time_ == UINTMAX_MAX) start_time_ = t;

    if ((t - start_time_) % di_ == 0) {
        if (!buffer_) {
            Package package = Package();
            push_package(std::move(package));
        }
    }
}


Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    id_ = id;
    processing_duration_ = pd;
    queue_ptr_ = std::move(q);
}


void Worker::do_work(Time time) {
    if (!currently_processed_package_.has_value() && !queue_ptr_->empty()) {
        currently_processed_package_.emplace(std::move(queue_ptr_->pop()));
        package_processing_start_time_ = time;
    }

    if (currently_processed_package_.has_value() && time >= package_processing_start_time_ + processing_duration_ - 1) {
        PackageSender::push_package(std::move(currently_processed_package_.value()));
        currently_processed_package_.reset();
    }
}


void Worker::receive_package(Package &&p) {
    queue_ptr_->push(std::move(p));
}