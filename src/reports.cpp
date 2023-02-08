//
// Created by brzan on 03.01.2023.
//

#include <string>
#include "reports.hpp"

void generate_structure_report(const Factory& factory, std::ostream& os) {
    os << std::endl << "== LOADING RAMPS ==" << std::endl;
    for (auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp++) {
        os << std::endl << "LOADING RAMP #" << ramp->get_id() << std::endl;
        os << "  Delivery interval: " << ramp->get_delivery_interval() << std::endl;
        os << "  Receivers:" << std::endl;
        auto prefs = ramp->receiver_preferences_.get_preferences();
        for (const auto& pref : prefs) {
            os << "    " << ((pref.first->get_receiver_type() == ReceiverType::WORKER) ? "worker" : "UNKNOWN") <<  " #" << pref.first->get_id() <<std::endl;
        }
    }
    os << std::endl << std::endl;

    os << "== WORKERS ==" << std::endl;
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        os << std::endl << "WORKER #" << worker->get_id() << std::endl;
        os << "  Processing time: " << worker->get_processing_duration() << std::endl;
        os << "  Queue type: " << ((worker->get_queue()->get_queue_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO") << std::endl;
        os << "  Receivers:" << std::endl;
        auto prefs = worker->receiver_preferences_.get_preferences();
        for (const auto& pref : prefs) {
            os << "    " << ((pref.first->get_receiver_type() == ReceiverType::STOREHOUSE) ? "storehouse" : "worker") << " #" << pref.first->get_id() << std::endl;
        }
    }
    os << std::endl << std::endl;

    os << "== STOREHOUSES ==" << std::endl;
    for (auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++) {
        os << std::endl << "STOREHOUSE #" << storehouse->get_id() << std::endl;
    }
    os << std::endl;
}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time time) {
    os << "=== [ Turn: " << time << " ] ===\n\n";
    os << "== WORKERS ==\n\n";
    for (auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++) {
        os << "WORKER #" << worker->get_id() << std::endl;
        os << "  PBuffer: " << ((worker->get_processing_buffer() == std::nullopt) ? "(empty)" : ("#" + std::to_string(worker->get_processing_buffer()->get_id())) + " (pt = " + std::to_string(time - worker->get_package_processing_start_time() + 1) + ")") << std::endl;
        if (worker->get_queue()->empty()) {
            os << "  Queue: (empty)\n";
        } else {
            os << "  Queue: ";
            for (auto package = worker->cbegin(); package != worker->cend(); package++) {
                if (package == std::prev(worker->cend())) {
                    os << "#" << package->get_id() << std::endl;
                } else {
                    os << "#" << package->get_id() << ", ";
                }
            }
        }
        os << "  SBuffer: " << ((worker->get_sending_buffer() == std::nullopt) ? "(empty)" : "#" + std::to_string(worker->get_sending_buffer()->get_id())) << std::endl << std::endl;
    }

    os << std::endl << "== STOREHOUSES ==\n\n";
    for (auto storehouse = factory.storehouse_cbegin(); storehouse != factory.storehouse_cend(); storehouse++) {
        os << "STOREHOUSE #" << storehouse->get_id() << std::endl;
        if (storehouse->cbegin() == storehouse->cend()) {
            os << "  Stock: (empty)" << std::endl;
        } else {
            os << "  Stock: ";
            for (auto package = storehouse->cbegin(); package!= storehouse->cend(); package++){
                if (package == std::prev(storehouse->cend())){
                    os << "#" << package->get_id() << std::endl;
                } else {
                    os << "#" << package->get_id() << ", ";
                }
            }
        }
    }
    os << std::endl;
}

bool IntervalReportNotifier::should_generate_report(Time time) const {
    if (timeoffset_ == 1) return true;
    if (time % timeoffset_ == 1) return true;
    return false;
}

bool SpecificTurnsReportNotifier::should_generate_report(Time time) const {
    return std::any_of(turns_.cbegin(), turns_.cend(), [time](TimeOffset turn){if (time == turn) return true; else return false;});
}
