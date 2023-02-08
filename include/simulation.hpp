// funkcja simulate()
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "reports.hpp"

void simulate(Factory& factory, TimeOffset rounds, std::function<void(Factory&, Time)> reportFunction);

#endif //NETSIM_SIMULATION_HPP
