
#include "transport_catalogue.h"
#include <algorithm>
#include <cmath>

namespace transport 
{
    void TransportCatalogue::AddStop(std::string_view stop_name, const geo::Coordinates coordinates) 
    { 
        stops_.push_back({std::string(stop_name), coordinates}); 
        stopname_to_stop_[stops_.back().name] = &stops_.back(); 
    } 

    void TransportCatalogue::AddBus(std::string_view bus_number, const std::vector<ConstStop> stops, bool is_circle) 
    { 
        buses_.push_back({std::string(bus_number), stops, is_circle}); 
        ConstBus bus = &buses_.back();
        busname_to_bus_[bus->number] = bus;
        
        for (ConstStop stop : stops) {
            stop_to_buses_[stop].insert(bus);
        }
    } 

    const Bus *TransportCatalogue::FindBus(std::string_view bus_number) const 
    { 
        return busname_to_bus_.count(bus_number) ? busname_to_bus_.at(bus_number) : nullptr; 
    } 
 
    const Stop *TransportCatalogue::FindStop(std::string_view stop_name) const 
    { 
        return stopname_to_stop_.count(stop_name) ? stopname_to_stop_.at(stop_name) : nullptr; 
    } 

    std::optional<BusStat> TransportCatalogue::GetBusStat(const std::string_view &bus_number) const 
    { 
        const Bus* bus = FindBus(bus_number);
        if (!bus) return std::nullopt;

        BusStat bus_stat{};
        bus_stat.stops_count = bus->is_circle ? bus->stops.size() : bus->stops.size() * 2 - 1;
        bus_stat.unique_stops_count = UniqueStopsCount(bus_number);

        double geographic_length = 0.0;
        int route_length = 0;

        for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
            ConstStop from = bus->stops[i];
            ConstStop to = bus->stops[i+1];
            
            geographic_length += geo::ComputeDistance(from->coordinates, to->coordinates);
            route_length += GetDistance(from, to);
            
            if (!bus->is_circle) {
                geographic_length += geo::ComputeDistance(to->coordinates, from->coordinates);
                route_length += GetDistance(to, from);
            }
        }

        bus_stat.route_length = route_length;
        bus_stat.curvature = route_length / geographic_length;

        return bus_stat;
    } 

    size_t TransportCatalogue::UniqueStopsCount(std::string_view bus_number) const 
    { 
        std::unordered_set<std::string_view> unique_stops; 
        for (const auto &stop : busname_to_bus_.at(bus_number)->stops) 
        { 
            unique_stops.insert(stop->name); 
        } 
        return unique_stops.size(); 
    } 

    const std::set<std::string> TransportCatalogue::GetBusesByStop(std::string_view stop_name) const 
    { 
        ConstStop stop = FindStop(stop_name);
        if (!stop) return {};
        
        auto it = stop_to_buses_.find(stop);
        if (it == stop_to_buses_.end()) return {};
        
        std::set<std::string> result;
        for (ConstBus bus : it->second) {
            result.insert(bus->number);
        }
        return result;
    } 

    void TransportCatalogue::SetDistance(ConstStop from, ConstStop to, const int distance) 
    { 
        stop_distances_[{from, to}] = distance; 
    } 

    int TransportCatalogue::GetDistance(const Stop *from, const Stop *to) const 
    { 
        if (stop_distances_.count({from, to})) 
            return stop_distances_.at({from, to}); 
        else if (stop_distances_.count({to, from})) 
            return stop_distances_.at({to, from}); 
        else 
            return 0; 
    } 
}