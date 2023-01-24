#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>
#include <vector>
#include <string>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern std::vector<std::string> splitString(std::string line, char c);
extern std::string removeWhitespaces(std::string data);

extern std::function<double()> probability_generator;

#endif /* HELPERS_HPP_ */