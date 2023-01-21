//
// Created by brzan on 03.01.2023.
//

#include "../include/nodes.hpp"

Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ips_ptr = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) {
    id_ = id;
    iPackageStockpile_ptr_ = std::move(ips_ptr);
}


void Storehouse::receive_package(Package &&p) {
    iPackageStockpile_ptr_->push(std::move(p));
}


void ReceiverPreferences::add_receiver(IPackageReceiver* r){
    preferences_.emplace(r,1);
    for(auto i = preferences_.begin(); i != preferences_.end(); i++){
        i->second = 1/(double)(preferences_.size());
    };
};


IPackageReceiver *ReceiverPreferences::choose_receiver(){
    double dys = probability_generator();
    double suma = 0;
    for(auto i = preferences_.begin(); i != preferences_.end(); i++){
        suma += i->second;
        if(suma > dys) {
            return i->first;
        };
    };
    return nullptr;
};


void ReceiverPreferences::remove_receiver(IPackageReceiver* r){
    preferences_.erase(r);
    if(preferences_.size() != 0){
        for(auto i = preferences_.begin();i != preferences_.end();i++){
            i->second = 1 / (double)(preferences_.size());
        };
    };
};


void PackageSender::send_package() {
    if(buffer_){
        receiver_preferences_.choose_receiver()->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}


Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    //todo Zapytac czy PackageSender moze miec bezargumentowy konstruktor czy nie, jak dla mnie jest lekka sprzecznosc
    id_ = id;
    processing_duration_ = pd;
    queue_ptr_ = std::move(q);
}


void Worker::do_work(Time time) { // moze byc kraksa ale w mojej glowie to dziala XD jak cos to prosze pisac XDD
    if (!currently_processed_package_.has_value()) {
        currently_processed_package_.emplace(std::move(queue_ptr_->pop()));
        package_processing_start_time_ = time;
    }

    if (time >= package_processing_start_time_ + processing_duration_) {
        // fixme Nie wiem czy lepiej bedzie wstawiac to do bufora czy ta metoda powinna to sama wyslac. Poki co wstawia do bufora
//        receiver_preferences_.choose_receiver()->receive_package(std::move(currently_processed_package_.value()));
//        currently_processed_package_.reset();
    buffer_.emplace(std::move(currently_processed_package_.value()));
    currently_processed_package_.reset();
    }
}


void Worker::receive_package(Package &&p) {
//    if (!currently_processed_package_.has_value()) {
//        currently_processed_package_.emplace(std::move(p));
//    }
//    else {
//        queue_ptr_->push(std::move(p));
//    }
    queue_ptr_->push(std::move(p));
}


