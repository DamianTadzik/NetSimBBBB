// klasy SpecificTurnsReportNotifier, IntervalReportNotifier
// funkcje generate_structure_report(), generate_simulation_turn_report()
//
// Created by brzan on 03.01.2023.
//

#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "factory.hpp"


class IntervalReportNotifier {
private:
    TimeOffset timeoffset_;

public:
    explicit IntervalReportNotifier(TimeOffset timeoffset) : timeoffset_(timeoffset) {};

    bool should_generate_report(Time time) const;
};


class SpecificTurnsReportNotifier {
private:
    std::set<Time> turns_;

public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {};

    bool should_generate_report(Time time) const;
};


void generate_structure_report(const Factory& factory, std::ostream& os);

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time time);

#endif //NETSIM_REPORTS_HPP
