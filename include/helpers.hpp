// Pomocnicze obiekty i funkcje globalne (np. generator liczb losowych)
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_HELPERS_HPP
#define NETSIM_HELPERS_HPP

#include "types.hpp"

#include <functional>
#include <random>
#include <cstdlib>
#include <vector>
#include <string>


using ProbabilityGenerator = std::function<double()>;

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

extern std::vector<std::string> splitString(std::string line, char c);

extern std::string removeWhitespaces(std::string data);

#endif //NETSIM_HELPERS_HPP
