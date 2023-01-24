// aliasy
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <functional>

using ElementID = int;
using size_type = int;

using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_TYPES_HPP
