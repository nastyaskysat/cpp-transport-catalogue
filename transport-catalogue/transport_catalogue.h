#pragma once

#include "geo.h"
#include "domain.h"

#include <iostream>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <optional>
#include <unordered_set>
#include <set>
#include <map>

namespace transport
{

    class Catalogue
    {
    public:
        // вернула алиасы
        using ConstBus = const Bus *;
        using ConstStop = const Stop *;
        struct StopDistancesHasher
        {
            size_t operator()(const std::pair<ConstStop, ConstStop> &points) const
            {
                size_t hash_first = std::hash<const void *>{}(points.first);
                size_t hash_second = std::hash<const void *>{}(points.second);
                return hash_first + hash_second * 37;
            }
        };

        void AddStop(std::string_view stop_name, const geo::Coordinates coordinates);
        void AddRoute(std::string_view bus_number, const std::vector<ConstStop> stops, bool is_circle);
        const Bus *FindRoute(std::string_view bus_number) const;
        const Stop *FindStop(std::string_view stop_name) const;
        size_t UniqueStopsCount(std::string_view bus_number) const;
        void SetDistance(const Stop *from, const Stop *to, const int distance);
        int GetDistance(const Stop *from, const Stop *to) const;
        std::optional<BusStat> GetBusStat(std::string_view bus_number) const;
        const std::map<std::string_view, ConstBus> GetSortedAllBuses() const;

    private:
        std::deque<Bus> all_buses_;
        std::deque<Stop> all_stops_;
        std::unordered_map<std::string_view, ConstBus> busname_to_bus_;
        std::unordered_map<std::string_view, ConstStop> stopname_to_stop_;
        std::unordered_map<std::pair<ConstStop, ConstStop>, int, StopDistancesHasher> stop_distances_;
    };

} // namespace transport