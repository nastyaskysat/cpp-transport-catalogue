
#include "stat_reader.h"
#include <iomanip>

namespace transport 
{
    void output_(std::istream& in, TransportCatalogue& catalogue, std::ostream& out) 
    {
        size_t requests_count;
        in >> requests_count;
        std::string dummy;
        std::getline(in, dummy);
        
        for (size_t i = 0; i < requests_count; ++i) {
            std::string line;
            std::getline(in, line);
            
            size_t space_pos = line.find(' ');
            if (space_pos == std::string::npos) continue;
            
            std::string keyword = line.substr(0, space_pos);
            std::string argument = line.substr(space_pos + 1);
            
            if (keyword == "Bus") {
                PrintRoute(argument, catalogue, out);
            } else if (keyword == "Stop") {
                PrintStop(argument, catalogue, out);
            }
        }
    }

    void PrintRoute(std::string& bus_number, TransportCatalogue& catalogue, std::ostream& out) 
    {
        bus_number.erase(0, bus_number.find_first_not_of(" \t"));
        bus_number.erase(bus_number.find_last_not_of(" \t") + 1);
        
        auto stat = catalogue.GetBusStat(bus_number);
        if (!stat) {
            out << "Bus " << bus_number << ": not found\n";
            return;
        }
        
        out << "Bus " << bus_number << ": " 
            << stat->stops_count << " stops on route, "
            << stat->unique_stops_count << " unique stops, "
            << stat->route_length << " route length, "
            << std::setprecision(6) << stat->curvature << " curvature\n";
    }

    void PrintStop(std::string& stop_name, TransportCatalogue& catalogue, std::ostream& out) 
    {
        stop_name.erase(0, stop_name.find_first_not_of(" \t"));
        stop_name.erase(stop_name.find_last_not_of(" \t") + 1);
        
        auto buses = catalogue.GetBusesByStop(stop_name);
        out << "Stop " << stop_name << ": ";
        
        if (buses.empty()) {
            if (catalogue.FindStop(stop_name)) {
                out << "no buses";
            } else {
                out << "not found";
            }
        } else {
            out << "buses";
            for (const auto& bus : buses) {
                out << " " << bus;
            }
        }
        out << "\n";
    }
}
