#pragma once

#include "transport_catalogue.h"

#include <iostream>

namespace transport
{

    void input_(std::istream &in, TransportCatalogue &catalogue);

    std::pair<std::string, geo::Coordinates> FillStop(std::string &line);
    void FillStopDistances(std::string &line, TransportCatalogue &catalogue);
    std::tuple<std::string, std::vector<const Stop *>, bool> FillRoute(std::string &line, transport::TransportCatalogue &catalogue);

} // namespace transport