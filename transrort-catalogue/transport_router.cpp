#include "transport_router.h" 

namespace transport {

Router::Router(const RoutingSettings& settings, const Catalogue& catalogue) 
    : settings_(settings), catalogue_(&catalogue)
{
    BuildGraph(catalogue);
}

void Router::BuildGraph(const Catalogue& catalogue) {
    const auto& all_stops = catalogue.GetSortedAllStops();
    const auto& all_buses = catalogue.GetSortedAllBuses();
    graph_ = graph::DirectedWeightedGraph<double>(all_stops.size() * 2);
    stop_ids_.clear();
    graph::VertexId vertex_id = 0;

    for (const auto& [stop_name, stop_info] : all_stops) {
        stop_ids_[stop_info->name] = vertex_id;
        graph_.AddEdge({
            stop_info->name,
            0,
            vertex_id,
            ++vertex_id,
            static_cast<double>(settings_.bus_wait_time)
        });
        ++vertex_id;
    }

    for (const auto& [bus_number, bus] : all_buses) {
        const auto& stops = bus->stops;
        size_t stops_count = stops.size();
        for (size_t i = 0; i < stops_count; ++i) {
            for (size_t j = i + 1; j < stops_count; ++j) {
                const Stop* stop_from = stops[i];
                const Stop* stop_to = stops[j];
                int dist_sum = 0;
                int dist_sum_inverse = 0;
                for (size_t k = i + 1; k <= j; ++k) {
                    dist_sum += catalogue.GetDistance(stops[k-1], stops[k]);
                    dist_sum_inverse += catalogue.GetDistance(stops[k], stops[k-1]);
                }
                graph_.AddEdge({
                    bus->number,
                    j - i,
                    stop_ids_.at(stop_from->name) + 1,
                    stop_ids_.at(stop_to->name),
                    dist_sum / (settings_.bus_velocity * 1000.0 / 60.0)
                });

                if (!bus->is_circle) {
                    graph_.AddEdge({
                        bus->number,
                        j - i,
                        stop_ids_.at(stop_to->name) + 1,
                        stop_ids_.at(stop_from->name),
                        dist_sum_inverse / (settings_.bus_velocity * 1000.0 / 60.0)
                    });
                }
            }
        }
    }

    router_ = std::make_unique<graph::Router<double>>(graph_);
}

std::optional<RouteInternalInfo> Router::FindRoute(const Stop* stop_from, const Stop* stop_to) const {
    if (!stop_ids_.count(stop_from->name) || !stop_ids_.count(stop_to->name)) {
        return std::nullopt;
    }
    
    auto route_info = router_->BuildRoute(
        stop_ids_.at(stop_from->name), 
        stop_ids_.at(stop_to->name)
    );
    
    if (!route_info) {
        return std::nullopt;
    }

    RouteInternalInfo result;
    result.total_time = Minutes{route_info->weight};

    for (const auto& edge_id : route_info->edges) {
        const auto& edge = graph_.GetEdge(edge_id);
        
        if (edge.quality == 0) { // Wait edge
            result.items.push_back(RouteInternalInfo::WaitItem{
                catalogue_->FindStop(edge.name),
                Minutes{edge.weight}
            });
        } else { // Bus edge
            result.items.push_back(RouteInternalInfo::BusItem{
                catalogue_->FindRoute(edge.name),
                Minutes{edge.weight},
                edge.quality
            });
        }
    }

    return result;
}

} // namespace transport