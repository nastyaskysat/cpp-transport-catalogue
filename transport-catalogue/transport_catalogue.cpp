#include "transport_catalogue.h"

#include <execution>

namespace transport_catalogue
{

    void TransportCatalogue::AddStop(std::string_view name, geo::Coordinates coordinates)
    {
        stops_.push_back({std::string(name), coordinates});
        const Stop *stop_buf = &stops_.back();
        stopname_to_stop[stop_buf->name_] = stop_buf;
    }

    void TransportCatalogue::AddBus(std::string_view name, const std::vector<std::string_view> &stop_names)
    {
        buses_.push_back({std::string(name), {}});
        Bus *bus_buf = &buses_.back();

        for (std::string_view stop_name : stop_names)
        {
            const Stop *stop = GetStop(stop_name);
            if (stop)
            {
                bus_buf->stops_.push_back(stop);
                stop_to_buses_[stop].insert(bus_buf);
            }
        }

        busname_to_bus[bus_buf->name_] = bus_buf;
    }

    const Bus *TransportCatalogue::GetBus(std::string_view bus_name) const
    {
        if (busname_to_bus.empty())
        {
            return nullptr;
        }

        try
        {
            return busname_to_bus.at(bus_name);
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }
    }

    const Stop *TransportCatalogue::GetStop(std::string_view stop_name) const
    {
        if (stopname_to_stop.empty())
        {
            return nullptr;
        }

        try
        {
            return stopname_to_stop.at(stop_name);
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }
    }

    std::unordered_set<ConstStop> TransportCatalogue::GetUniqueStops(const Bus *bus) const
    {
        std::unordered_set<ConstStop> unique_stops_;
        unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());
        return unique_stops_;
    }

    double TransportCatalogue::GetLenght(const Bus *bus) const
    {
        return transform_reduce(next(bus->stops_.begin()),
                                bus->stops_.end(),
                                bus->stops_.begin(),
                                0.0,
                                std::plus<>{},
                                [](const Stop *lhs, const Stop *rhs)
                                {
                                    return geo::ComputeDistance(lhs->coordinates, rhs->coordinates);
                                });
    }

    std::unordered_set<ConstBus> TransportCatalogue::StopGetUniqBuses(const Stop *stop) const
    {
        if (stop_to_buses_.count(stop))
        {
            return stop_to_buses_.at(stop);
        }
        return {};
    }
}