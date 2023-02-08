//
// Created by brzan on 03.01.2023.
//

#include "simulation.hpp"

void simulate(Factory& factory, TimeOffset rounds, std::function<void(Factory&, Time)> reportFunction) {
    if (!factory.is_consistent()) throw std::logic_error("Factory not consistent");

    for (Time round = 1; round <= rounds; round++) {
        factory.do_deliveries(round);
        factory.do_package_passing();
        factory.do_work(round);
        reportFunction(factory, round);
    }
}
