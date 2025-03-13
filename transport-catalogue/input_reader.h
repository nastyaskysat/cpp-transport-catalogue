#pragma once
#include "transport_catalogue.h"

namespace transport_catalogue
{

    namespace input
    {
        Stop SplitStop(std::string str);
        Bus SplitBus(TransportCatalogue &catalogue, std::string_view str);
        void input_(TransportCatalogue &catalogue);
    }
}