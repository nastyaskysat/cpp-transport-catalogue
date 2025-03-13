#include "stat_reader.h"

namespace transport_catalogue
{

    namespace output
    {

        void query_bus(TransportCatalogue &catalogue, std::string_view str)
        {
            auto entry = 4;
            str = str.substr(entry);

            const Bus *bus = catalogue.GetBus(str);
            if (bus != nullptr)
            {
                std::cout << "Bus " << bus->name_ << ": "
                          << bus->stops_.size() << " stops on route, "
                          << (catalogue.GetUniqueStops(bus)).size() << " unique stops, "
                          << std::setprecision(6) << catalogue.GetLenght(bus) << " route length" << std::endl;
            }
            else
            {
                std::cout << "Bus " << str << ": not found" << std::endl;
            }
        }

        void query_stop(TransportCatalogue &catalogue, std::string_view stop_name)
        {
            auto entry = 5;
            stop_name = stop_name.substr(entry);
            std::unordered_set<const Bus *> unique_buses;
            std::unordered_set<std::string_view> unique_buses_name;
            std::vector<std::string> bus_name_v;

            const Stop *stop = catalogue.GetStop(stop_name);

            if (stop != NULL)
            {
                unique_buses = catalogue.StopGetUniqBuses(stop);

                if (unique_buses.size() == 0)
                {
                    std::cout << "Stop " << stop_name << ": no buses" << std::endl;
                }
                else
                {
                    std::cout << "Stop " << stop_name << ": buses ";

                    for (const Bus *_bus : unique_buses)
                    {
                        bus_name_v.push_back(_bus->name_);
                    }

                    std::sort(bus_name_v.begin(), bus_name_v.end());

                    for (std::string_view bus_name : bus_name_v)
                    {
                        std::cout << bus_name;
                        std::cout << " ";
                    }
                    std::cout << std::endl;
                }
            }
            else
            {
                std::cout << "Stop " << stop_name << ": not found" << std::endl;
            }
        }

        void query_(TransportCatalogue &catalogue, std::string_view str)
        {
            if (str.substr(0, 3) == "Bus")
            {
                query_bus(catalogue, str);
            }
            else if (str.substr(0, 4) == "Stop")
            {
                query_stop(catalogue, str);
            }
            else
            {
                std::cout << "Error query" << std::endl;
            }
        }

        void output_(TransportCatalogue &catalogue)
        {
            std::string count;
            std::getline(std::cin, count);

            std::string str;
            std::vector<std::string> query;
            auto amount = stoi(count);

            for (int i = 0; i < amount; ++i)
            {
                std::getline(std::cin, str);
                query.push_back(str);
            }

            for (auto &strr : query)
            {
                query_(catalogue, strr);
            }
        }
    }
}
