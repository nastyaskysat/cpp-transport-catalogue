#pragma once
#include "transport_catalogue.h"

namespace transport_catalogue
{

    namespace input
    {
        Stop SplitStop(std::string_view str);
        Bus SplitBus(TransportCatalogue &catalogue, std::string_view str);
        void input_(TransportCatalogue &catalogue);
    }
}