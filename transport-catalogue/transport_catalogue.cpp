#include "transport_catalogue.h"

#include <execution>


namespace transport_catalogue
{

    void TransportCatalogue::AddStop(Stop &&stop_)
    {
        stops_.push_back(std::move(stop_));
        Stop *stop_buf = &stops_.back();
        stopname_to_stop.insert(StopMap::value_type(stop_buf->name_, stop_buf));
    }

    void TransportCatalogue::AddBus(Bus &&bus_)
    {
        Bus *bus_buf;
        buses_.push_back(std::move(bus_));
        bus_buf = &buses_.back();
        busname_to_bus.insert(BusMap::value_type(bus_buf->name_, bus_buf));

        for (Stop *_stop : bus_buf->stops_)
        {
            _stop->buses_.push_back(bus_buf);
        }
    }

    Bus *TransportCatalogue::GetBus(std::string_view bus_name)
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

    Stop *TransportCatalogue::GetStop(std::string_view stop_name)
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

    std::unordered_set<const Stop*> TransportCatalogue::GetUniqueStops(Bus *bus)
    {
        std::unordered_set<const Stop*> unique_stops_;

        unique_stops_.insert(bus->stops_.begin(), bus->stops_.end());

        return unique_stops_;
    }

    double TransportCatalogue::GetLenght(Bus *bus)
    {
        return transform_reduce(next(bus->stops_.begin()),
                                bus->stops_.end(),
                                bus->stops_.begin(),
                                0.0,
                                std::plus<>{},
                                [](const Stop *lhs, const Stop *rhs)
                                {
                                    return geo::ComputeDistance({(*lhs).lat_,
                                                            (*lhs).lng_},
                                                           {(*rhs).lat_,
                                                            (*rhs).lng_});
                                });
    }

    std::unordered_set<const Bus *> TransportCatalogue::StopGetUniqBuses(Stop *stop)
    {
        std::unordered_set<const Bus*> unique_stops_;

        unique_stops_.insert(stop->buses_.begin(), stop->buses_.end());

        return unique_stops_;
    }
}