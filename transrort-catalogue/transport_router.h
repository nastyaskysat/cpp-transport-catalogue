#pragma once 

#include "router.h"
#include "transport_catalogue.h"

#include <memory>
#include <chrono>
#include <variant>

namespace transport {

using Minutes = std::chrono::duration<double, std::chrono::minutes::period>;

struct RouteInternalInfo {
    Minutes total_time;

    struct BusItem {
        const Bus* bus_ptr;
        Minutes time;
        size_t span_count;
    };
    struct WaitItem {
        const Stop* stop_ptr;
        Minutes time;
    };

    using Item = std::variant<BusItem, WaitItem>;
    std::vector<Item> items;
};

class Router {
public:
 Router(int bus_wait_time, double bus_velocity, const Catalogue& catalogue)
        : settings_{bus_wait_time, bus_velocity}, catalogue_(&catalogue) 
    {
        BuildGraph(catalogue);
    }
    Router(const RoutingSettings& settings, const Catalogue& catalogue);
    
    std::optional<RouteInternalInfo> FindRoute(const Stop* stop_from, const Stop* stop_to) const;

private:
    void BuildGraph(const Catalogue& catalogue);
    
    RoutingSettings settings_;
    const Catalogue* catalogue_ = nullptr;
    graph::DirectedWeightedGraph<double> graph_;
    std::map<std::string, graph::VertexId> stop_ids_;
    std::unique_ptr<graph::Router<double>> router_;
};

} // namespace transport