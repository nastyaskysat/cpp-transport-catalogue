#include "transport_catalogue.h"

namespace transport
{

    void TransportCatalogue::AddStop(std::string_view stop_name, const geo::Coordinates coordinates)
    {
        stops_.push_back({std::string(stop_name), coordinates, {}});
        stopname_to_stop_[stops_.back().name] = &stops_.back();
    }

    void TransportCatalogue::AddBus(std::string_view bus_number, const std::vector<const Stop *> stops, bool is_circle)
    {
        buses_.push_back({std::string(bus_number), stops, is_circle});
        busname_to_bus_[buses_.back().number] = &buses_.back();
        for (const auto &route_stop : stops)
        {
            for (auto &stop_ : stops_)
            {
                if (stop_.name == route_stop->name)
                    stop_.buses_by_stop.insert(std::string(bus_number));
            }
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
        BusStat bus_stat{};
        const Bus *bus = FindBus(bus_number);

        if (!bus)
            throw std::invalid_argument("bus not found");
        if (bus->is_circle)
            bus_stat.stops_count = bus->stops.size();
        else
            bus_stat.stops_count = bus->stops.size() * 2 - 1;

        int route_length = 0;
        double geographic_length = 0.0;

        for (size_t i = 0; i < bus->stops.size() - 1; ++i)
        {
            auto from = bus->stops[i];
            auto to = bus->stops[i + 1];
            if (bus->is_circle)
            {
                route_length += GetDistance(from, to);
                geographic_length += geo::ComputeDistance(from->coordinates,
                                                          to->coordinates);
            }
            else
            {
                route_length += GetDistance(from, to) + GetDistance(to, from);
                geographic_length += geo::ComputeDistance(from->coordinates,
                                                          to->coordinates) *
                                     2;
            }
        }

        bus_stat.unique_stops_count = UniqueStopsCount(bus_number);
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
        return stopname_to_stop_.at(stop_name)->buses_by_stop;
    }

    void TransportCatalogue::SetDistance(const Stop *from, const Stop *to, const int distance)
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

} // namespace transport