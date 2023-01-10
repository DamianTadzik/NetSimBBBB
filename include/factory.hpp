// szablon klasy NodeCollection
// klasa Factory
// funkcje do zapisu i odczytu struktury fabryki z pliku
//
// Created by brzan on 03.01.2023.
//
#include "types.hpp"

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

class Factory{
    void add_ramp(Ramp&&);
    void remove_ramp(ElementID id);
};


#endif //NETSIM_FACTORY_HPP
