//
// Created by brzan on 03.01.2023.
//

#include "factory.hpp"
#include <exception>
#include <string>
#include <sstream>


bool has_reachable_storehouse(PackageSender *sender, std::map<PackageSender *, SenderStatus> &statuses) {
    if (statuses[sender] == SenderStatus::Verified)
        return true;

    statuses[sender] = SenderStatus::Visited;

    if (sender->receiver_preferences_.begin() == sender->receiver_preferences_.end())
        throw DExcept();

    bool alo = false; // czy nadawca ma choc jednego odbiore innego od siebie samego
    auto prefs = sender->receiver_preferences_;

    for (auto r: prefs.get_preferences()) {
        auto type = r.first->get_receiver_type();
        if (type == ReceiverType::STOREHOUSE) {
            alo = true;
        } else if (type == ReceiverType::WORKER) {
            IPackageReceiver *r_ptr = r.first;
            auto w_ptr = dynamic_cast<Worker *>(r_ptr);
            auto s_ptr = dynamic_cast<PackageSender *>(w_ptr);
            if (s_ptr == sender || s_ptr == nullptr)
                continue;

            alo = true;
            if (statuses[s_ptr] == SenderStatus::NotVisited)
                has_reachable_storehouse(s_ptr, statuses);
        }
    }

    statuses[sender] = SenderStatus::Verified;
    if (alo)
        return true;
    else
        throw DExcept();
}

bool Factory::is_consistent() {
    std::map<PackageSender *, SenderStatus> statuses;

    for (auto it = &(*workerCollection.begin()), end = &(*workerCollection.end()); it < end; it++)
        statuses.insert({it, SenderStatus::NotVisited});

    for (auto it = &(*rampCollection.begin()), end = &(*rampCollection.end()); it < end; it++)
        statuses.insert({it, SenderStatus::NotVisited});

    try {
        for (auto it = &(*rampCollection.begin()), end = &(*rampCollection.end()); it < end; it++)
            if (!has_reachable_storehouse(dynamic_cast<PackageSender *>(it), statuses))
                return false;
        return true;
    }
    catch (const DExcept &) {
        return false;
    }
    throw DExcept();
}

void Factory::do_deliveries(Time t) {
    for (auto i = rampCollection.begin(); i < rampCollection.end(); i++)
        i->deliver_goods(t);
}

void Factory::do_package_passing() {
    for (auto i = rampCollection.begin(); i < rampCollection.end(); i++)
        i->send_package();

    for (auto i = workerCollection.begin(); i < workerCollection.end(); i++)
        i->send_package();
}

void Factory::do_work(Time t) {
    for (auto i = workerCollection.begin(); i < workerCollection.end(); i++)
        i->do_work(t);
}

void Factory::remove_receiver(NodeCollection<Ramp> &collection, ElementID id) {
    for (auto &section: collection) {
        auto prefs = section.receiver_preferences_.get_preferences();
        for (auto pref: prefs) {
            if (pref.first->get_id() == workerCollection.find_by_id(id)->get_id()) {
                section.receiver_preferences_.remove_receiver(pref.first);
                break;
            }
        }
    }
}

void Factory::remove_receiver(NodeCollection<Worker> &collection, ElementID id) {
    //moze zle mysle ale moze byc tak ze jak worker i storehouse maja to samo id to usunie i workera i storehouse
    for (auto &section: collection) {
        auto prefs = section.receiver_preferences_.get_preferences();
        for (auto pref: prefs) {
            if (pref.first->get_id() == id) {
                section.receiver_preferences_.remove_receiver(pref.first);
                break;
            }
        }
    }
}

// Ramp
void Factory::add_ramp(Ramp &&r) {
    rampCollection.add(std::move(r));
}

void Factory::remove_ramp(ElementID id) {
    rampCollection.remove_by_id(id);
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return rampCollection.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() const {
    return rampCollection.cbegin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() const {
    return rampCollection.cend();
}

// Worker
void Factory::add_worker(Worker &&w) {
    workerCollection.add(std::move(w));
}

void Factory::remove_worker(ElementID id) {
    //workerCollection.remove_by_id(id);
//    for (auto ramp = rampCollection.begin(); ramp != rampCollection.end(); ramp++) {
//        auto prefs = ramp->receiver_preferences_.get_preferences();
//
//        for (auto receiver: prefs) {
//            if (receiver.first->get_id() == id) {
//                ramp->receiver_preferences_.remove_receiver(receiver.first);
//                break;
//            }
//        }
//    }
    remove_receiver(rampCollection, id);
    remove_receiver(workerCollection, id);
    workerCollection.remove_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return workerCollection.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() const {
    return workerCollection.cbegin();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend() const {
    return workerCollection.cend();
}


// Storehouse
void Factory::add_storehouse(Storehouse &&s) {
    storehouseCollection.add(std::move(s));
}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(workerCollection, id);
    storehouseCollection.remove_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return storehouseCollection.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() const {
    return storehouseCollection.cbegin();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() const {
    return storehouseCollection.cend();
}

struct Argument {
    std::string title;
    std::string value;

    Argument(std::string t, std::string v) : title(t), value(v) {}
};

struct LineParser {
private:
    std::function<bool(Factory &, std::vector<Argument>)> parserFunc;
public:

    std::string tag;
    size_t argsCount;

    LineParser(std::string t, size_t ac, std::function<bool(Factory &, std::vector<Argument>)> f) : parserFunc(f),
                                                                                                    tag(t),
                                                                                                    argsCount(ac) {}

    bool Execute(Factory &f, std::vector<Argument> args) {
        if (args.size() != argsCount) {
            std::cout << "Wrong arguments count" << std::endl;
            return false;
        }
        return parserFunc(f, args);
    }
};

template<class T>
T S2T(std::string s) {
    std::stringstream ss(s);
    T n;
    ss >> n;

    return n;
}

bool ParseRamp(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset deliveryInterval = S2T<TimeOffset>(args[1].value);

    f.add_ramp(Ramp(id, deliveryInterval));

    return true;
}

bool ParseWorker(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);
    TimeOffset processingTime = S2T<TimeOffset>(args[1].value);

    PackageQueueType type = PackageQueueType::FIFO;

    if (args[2].value == "FIFO") {
        type = PackageQueueType::FIFO;
    } else if (args[2].value == "LIFO") {
        type = PackageQueueType::LIFO;
    } else {
        std::cout << "Unknown queue type" << std::endl;
        return false;
    }

    f.add_worker(Worker(id, processingTime, std::make_unique<PackageQueue>(type)));

    return true;
}

bool ParseStorehouse(Factory &f, std::vector<Argument> args) {
    ElementID id = S2T<ElementID>(args[0].value);

    f.add_storehouse(Storehouse(id));

    return true;
}

bool ParseLink(Factory &f, std::vector<Argument> args) {
    std::string src = args[0].value;
    std::string dest = args[1].value;

    std::vector<std::string> srcSplitted = splitString(src, '-');
    std::vector<std::string> destSplitted = splitString(dest, '-');

    PackageSender *srcObject = nullptr;
    IPackageReceiver *destObject = nullptr;

    ElementID srcId = S2T<ElementID>(srcSplitted[1]);
    ElementID destId = S2T<ElementID>(destSplitted[1]);
    if (srcSplitted[0] == "ramp")
        srcObject = &*f.find_ramp_by_id(srcId);
    else if (srcSplitted[0] == "worker")
        srcObject = &*f.find_worker_by_id(srcId);
    else {
        std::cout << "Unknown source object type" << std::endl;
        return false;
    }

    if (destSplitted[0] == "worker")
        destObject = &*f.find_worker_by_id(destId);
    else if (destSplitted[0] == "store")
        destObject = &*f.find_storehouse_by_id(destId);
    else {
        std::cout << "Unknown destination object type" << std::endl;
        return false;
    }

    srcObject->receiver_preferences_.add_receiver(destObject);

    return true;
}

std::array<LineParser, 4> parsers = {
        LineParser("LOADING_RAMP", 2, ParseRamp),
        LineParser("WORKER", 3, ParseWorker),
        LineParser("STOREHOUSE", 1, ParseStorehouse),
        LineParser("LINK", 2, ParseLink),
};

std::vector<Argument> GenerateArgs(std::vector<std::string> parts, int startIndex = 1) {
    std::vector<Argument> args;
    for (size_t i = startIndex; i < parts.size(); i++) {
        auto p = splitString(parts[i], '=');
        if (p.size() != 2) {
            std::cout << "Error getting argument from \"" << parts[i] << "\" - wrong split string size" << std::endl;
            return std::vector<Argument>();
        }

        args.push_back(Argument(p[0], p[1]));
    }

    return args;
}

bool ParseLine(Factory &f, std::string line) {
    line = removeWhitespaces(line);

    if (line.size() == 0 || line[0] == ';' || line[0] == ' ')
        return true;

    auto data = splitString(line, ' ');
    std::string tag = data[0];
    for (size_t i = 0; i < parsers.size(); i++) {
        if (parsers[i].tag == tag) {
            auto args = GenerateArgs(data);
            return parsers[i].Execute(f, args);
        }
    }

    std::cout << "Parser not found for line \"" << line << "\"" << std::endl;
    return false;
}

Factory load_factory_structure(std::istream &is) {
    Factory f;

    std::string s = "";
    for (int i = 0; getline(is, s); i++) {
        try {
            if (!ParseLine(f, s))
                std::cout << "Error parsing line at " << i + 1 << std::endl;
        }
        catch (const std::exception &e) {
            std::cout << "Exception thrown while parsing file at line " << i + 1 << std::endl;
        }

    }

    return f;
}

void save_factory_structure(Factory &fact, std::ostream &os) {
    Factory &f = (Factory &) fact;

    os << std::endl << "; == LOADING RAMPS ==" << std::endl << std::endl;
    for (auto i = f.ramp_cbegin(); i < f.ramp_cend(); i++) {
        os << "LOADING_RAMP id=" << i->get_id() << " delivery-interval=" << i->get_delivery_interval() << std::endl;
    }

    os << std::endl << "; == WORKERS ==" << std::endl << std::endl;
    for (auto i = f.worker_cbegin(); i < f.worker_cend(); i++) {
        os << "WORKER id=" << i->get_id() << " processing-time=" << i->get_processing_duration() << " queue-type=";
        switch (i->get_queue()->get_queue_type()) {
            case PackageQueueType::FIFO:
                os << "FIFO" << std::endl;
                break;

            case PackageQueueType::LIFO:
                os << "LIFO" << std::endl;
                break;
        }
    }

    os << std::endl << "; == STOREHOUSES ==" << std::endl << std::endl;
    for (auto i = f.storehouse_cbegin(); i < f.storehouse_cend(); i++) {
        os << "STOREHOUSE id=" << i->get_id() << std::endl;
    }

    os << std::endl << "; == LINKS ==" << std::endl << std::endl;


    for (auto it = f.ramp_cbegin(); it < f.ramp_cend(); it++) {
        bool ok = false;
        auto prefs = it->receiver_preferences_.get_preferences();
        for (auto el = prefs.begin(); el != prefs.end(); el++) {
            ok = true;
            os << "LINK src=ramp-" << it->get_id() << " dest=";
            std::string typeStr = "";
            switch (el->first->get_receiver_type()) {
                case ReceiverType::STOREHOUSE:
                    typeStr = "store";
                    break;

                case ReceiverType::WORKER:
                    typeStr = "worker";
                    break;
            }

            os << typeStr << "-" << el->first->get_id() << std::endl;
        }

        if (ok)
            os << std::endl;
    }

    for (auto it = f.worker_cbegin(); it < f.worker_cend(); it++) {
        bool ok = false;
        auto prefs = it->receiver_preferences_.get_preferences();
        for (auto el = prefs.begin(); el != prefs.end(); el++) {
            ok = true;
            os << "LINK src=worker-" << it->get_id() << " dest=";
            std::string typeStr = "";
            switch (el->first->get_receiver_type()) {
                case ReceiverType::STOREHOUSE:
                    typeStr = "store";
                    break;

                case ReceiverType::WORKER:
                    typeStr = "worker";
                    break;
            }

            os << typeStr << "-" << el->first->get_id() << std::endl;
        }

        if (ok)
            os << std::endl;
    }
}
