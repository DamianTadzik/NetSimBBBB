#include <iostream>

#include "simulation.hpp"

int main() {
    Factory factory;
    factory.add_ramp(Ramp(1, 3));
    factory.add_ramp(Ramp(2, 2));
    factory.add_worker(Worker(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
    factory.add_worker(Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
    factory.add_storehouse(Storehouse(1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp &ramp = *(factory.find_ramp_by_id(1));
    ramp.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(1))));

    Ramp &ramp2 = *(factory.find_ramp_by_id(2));
    ramp2.receiver_preferences_.add_receiver(&*factory.find_worker_by_id(1));
    ramp2.receiver_preferences_.add_receiver(&*factory.find_worker_by_id(2));

    Worker &worker = *(factory.find_worker_by_id(1));
    worker.receiver_preferences_.add_receiver(&*factory.find_worker_by_id(1));
    worker.receiver_preferences_.add_receiver(&*factory.find_worker_by_id(2));

    Worker &worker2 = *(factory.find_worker_by_id(2));
    worker2.receiver_preferences_.add_receiver(&*factory.find_storehouse_by_id(1));

    IntervalReportNotifier inte_notifier(25);
    SpecificTurnsReportNotifier spec_notifier(std::set<Time>{1});
    simulate(factory, 150, [&](Factory &f, TimeOffset t_offset) {
        if (spec_notifier.should_generate_report(t_offset)) { generate_structure_report(f, std::cout); }
        if (inte_notifier.should_generate_report(t_offset)) { generate_simulation_turn_report(f, std::cout, t_offset);}
    });

    return 0;
}
