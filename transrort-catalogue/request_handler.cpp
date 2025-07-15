#include "request_handler.h" 

std::optional<transport::BusStat> RequestHandler::GetBusStat(std::string_view bus_number) const { 
    return catalogue_.GetBusStat(bus_number); 
} 
 
const std::set<std::string> RequestHandler::GetBusesByStop(std::string_view stop_name) const { 
    return catalogue_.FindStop(stop_name)->buses_by_stop; 
} 
 
bool RequestHandler::IsBusNumber(std::string_view bus_number) const { 
    return catalogue_.FindRoute(bus_number); 
} 
 
bool RequestHandler::IsStopName(std::string_view stop_name) const { 
    return catalogue_.FindStop(stop_name); 
} 
 
std::optional<transport::RouteInternalInfo> RequestHandler::GetOptimalRoute(std::string_view stop_from, std::string_view stop_to) const { 
    const auto* from = catalogue_.FindStop(stop_from); 
    const auto* to = catalogue_.FindStop(stop_to); 
    if (!from || !to) { 
        return std::nullopt; 
    } 
    return router_.FindRoute(from, to); 
} 
 
svg::Document RequestHandler::RenderMap() const { 
    return renderer_.GetSVG(catalogue_.GetSortedAllBuses()); 
}