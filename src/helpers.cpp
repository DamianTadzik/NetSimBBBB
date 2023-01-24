#include "helpers.hpp"

#include <cstdlib>
#include <random>
#include <sstream>

// Do generowania wysokiej jakości ciągów liczb pseudolosowych warto użyć
// zaawansowanych generatorów, np. algorytmu Mersenne Twister.
// zob. https://en.cppreference.com/w/cpp/numeric/random
std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    // Generuj liczby pseudolosowe z przedziału [0, 1); 10 bitów losowości.
    return std::generate_canonical<double, 10>(rng);
}

std::vector<std::string> splitString(std::string line, char c)
{
    std::istringstream ss(line);

    std::string token = "";
    std::vector<std::string> ret;
    while(getline(ss, token, c))
        ret.push_back(token);

    return ret;
}

std::array<char, 2> whitespaceChars = {'\r', '\0'};
std::string removeWhitespaces(std::string data)
{
    for(size_t i = 0;i<data.size();i++)
        for(size_t j = 0;j<whitespaceChars.size();j++)
            if(data[i] == whitespaceChars[j])
            {
                data.erase(i--);
                break;
            }

    return data;
}

std::function<double()> probability_generator = default_probability_generator;
