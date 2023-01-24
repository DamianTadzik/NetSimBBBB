#include <fstream>
#include "nodes.hpp"
#include <iostream>

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP
#endif //NETSIM_FACTORY_HPP



template<class T>
class NodeCollection {
public:
    using container_t = typename std::vector<T>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

private:
    container_t collection;
public:

    NodeCollection() {}

    NodeCollection(container_t c) : collection(c) {}

    NodeCollection<T>::iterator begin() { return collection.begin(); }

    NodeCollection<T>::iterator end() { return collection.end(); }

    void add(T&& node) { collection.push_back(std::move(node)); }

    NodeCollection<T>::iterator find_by_id(ElementID id) {
        for (auto it = begin(); it < end(); it++) {
            if (it->get_id() == id)
                return it;
        }

        // Element not found
        std::cout << "NodeCollection::find_by_id(): Element with id " << id << " not found" << std::endl;
        return end();
    }

    NodeCollection<T>::iterator find_by_id(ElementID id) const {
        for (auto it = begin(); it < end(); it++) {
            auto el = *it;
            auto tId = el.get_id();

            if (tId == id)
                return it;
        }

        // Element not found
        std::cout << "NodeCollection::find_by_id(): Element with id " << id << " not found" << std::endl;
        return end();
    }

    void remove_by_iter(iterator it) {
        if (it != end())
            collection.erase(it);
    }

    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != end())
            collection.erase(it);
    }
};

class Factory {
private:
//    template<class T>
//    void remove_receiver(NodeCollection<T>& collection, ElementID id);

    NodeCollection<Ramp> rampCollection;
    NodeCollection<Worker> workerCollection;
    NodeCollection<Storehouse> storehouseCollection;

public:
    bool is_consistent();

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

    void add_ramp(Ramp&&);

    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator ramp_cbegin();

    NodeCollection<Ramp>::const_iterator ramp_cend();

    // dla pozostałych wezlow ma być niby analogicznie
    void add_worker(Worker&&);

    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);

    NodeCollection<Worker>::const_iterator worker_cbegin();

    NodeCollection<Worker>::const_iterator worker_cend();

    void add_storehouse(Storehouse&&);

    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin();

    NodeCollection<Storehouse>::const_iterator storehouse_cend();
};

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& f, std::ostream& os);