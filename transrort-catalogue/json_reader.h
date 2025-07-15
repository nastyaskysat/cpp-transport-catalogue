#pragma once  

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "request_handler.h"

#include <iostream>
#include <tuple>

class JsonReader {
public:
    JsonReader(std::istream& input, transport::Catalogue& catalogue);
    
 void FillCatalogue();
    const json::Node& GetBaseRequests() const;
    const json::Node& GetStatRequests() const;
    const json::Node& GetRenderSettings() const;
    const json::Node& GetRoutingSettings() const;

    void ProcessRequests(const json::Node& stat_requests, RequestHandler& rh) const;
    
    renderer::MapRenderer FillRenderSettings(const json::Node& settings) const;
    transport::Router FillRoutingSettings(const json::Node& settings) const;

    const json::Node PrintRoute(const json::Dict& request_map, RequestHandler& rh) const;
    const json::Node PrintStop(const json::Dict& request_map, RequestHandler& rh) const;
    const json::Node PrintMap(const json::Dict& request_map, RequestHandler& rh) const;
    const json::Node PrintRouting(const json::Dict& request_map, RequestHandler& rh) const;

private:
    json::Document input_;
    transport::Catalogue& catalogue_;
    json::Node dummy_ = nullptr;

    std::tuple<std::string_view, geo::Coordinates, std::map<std::string_view, int>> FillStop(const json::Dict& request_map) const;
    void FillStopDistances() const;
    std::tuple<std::string_view, std::vector<const transport::Stop*>, bool> FillRoute(const json::Dict& request_map) const;
};