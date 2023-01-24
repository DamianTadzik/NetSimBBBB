//
// Created by brzan on 03.01.2023.
//
#include "nodes.hpp"
#include <stdexcept>
#include <iostream>

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
        :IPackageReceiver(id, ReceiverType::WORKER)
{
    PackageSender();
    this->id = id;
    this->offset = pd;
    this->queue = std::move(q);
    this->start_time = 0;
}

ReceiverPreferences::ReceiverPreferences(ProbabilityGenerator pg)
{
    this->gen = pg;
}

ReceiverPreferences::ReceiverPreferences()
        :gen(default_probability_generator) {}

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    preferences.insert(std::pair<IPackageReceiver*,double>(r, 1.0/double(preferences.size())));
    for(auto i = preferences.begin(); i != preferences.end(); i++)
    {
        i->second = double(1.0/(preferences.size()));
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    preferences.erase(r);
    for(auto i = preferences.begin(); i != preferences.end(); i++)
    {
        i->second = double(1/(preferences.size()));
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    double lottery = gen();
    for(auto i = preferences.begin(); i!=preferences.end(); i++){
        if (lottery < i->second)
        {
            return i->first;
        }
        else
        {
            lottery -= i->second;
        }
    }
    throw std::invalid_argument("reached end of function choose_receiver: probably empty list");
}

ReceiverPreferences::preferences_t ReceiverPreferences::get_preferences() const
{
    return preferences;
}

void PackageSender::push_package(Package&& p)
{
    opt_.emplace(std::move(p));
}

void PackageSender::send_package()
{
    IPackageReceiver* package_receiver = receiver_preferences_.choose_receiver();
    package_receiver->receive_package(std::move(opt_.value()));
    opt_.reset();
}

std::optional<Package>& PackageSender::get_sending_buffer()
{
    return opt_;
}

void Ramp::deliver_goods(Time t)
{
    push_package(Package());
    if(d_t + offset <= t)
    {
        send_package();
        d_t = t;
    }
}

void Worker::do_work(Time t)
{
    if(!cur_package && queue)
    {
        cur_package.emplace(queue->pop());
        start_time = t;
    }
    if(cur_package && t + 1 - start_time >= offset)
    {
        push_package(std::move(cur_package.value()));
        cur_package.reset();
        start_time = 0;
    }
}

void Worker::receive_package(Package &&p) {
    if(!get_sending_buffer())
    {
        queue->push(std::move(p));
    }
    else
    {
        queue->push(std::move(p));
        push_package(queue->pop());
    }
}

TimeOffset Worker::get_processing_duration() const
{
    return offset;
}

Time Worker::get_package_processing_start_time()
{
    return start_time;
}

void Storehouse::receive_package(Package &&p) {
    queue-> push(std::move(p));
}