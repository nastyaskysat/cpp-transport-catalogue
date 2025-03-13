#include "input_reader.h"
#include "stat_reader.h"
#include <algorithm>

namespace transport_catalogue
{
    namespace input
    {
        Stop SplitStop(std::string str)
        {
            auto twopoint_pos = str.find(':');
            auto comma_pos = str.find(',');
            auto entry_length = 5;
            auto distance = 2;
            Stop _stop;

            _stop.name_ = str.substr(entry_length,
                                     twopoint_pos - entry_length);
            _stop.coordinates.lat = stod(str.substr(twopoint_pos + distance,
                                                    comma_pos - twopoint_pos - distance));
            _stop.coordinates.lng = stod(str.substr(comma_pos + distance));

            return _stop;
        }

        Bus SplitBus(TransportCatalogue &catalogue, std::string_view str)
        {
            auto entry_length = 4;
            auto distance = 2;
            auto twopoint_pos = str.find(':');
            Bus bus;
            bus.name_ = std::string(str.substr(entry_length, twopoint_pos - entry_length));

            str = str.substr(twopoint_pos + distance);

            auto more_pos = str.find('>');
            if (more_pos == std::string_view::npos)
            {
                auto tire_pos = str.find('-');

                while (tire_pos != std::string_view::npos)
                {
                    const auto stop_name = str.substr(0, tire_pos - 1);
                    if (const Stop *stop = catalogue.GetStop(stop_name))
                    {
                        bus.stops_.push_back(stop);
                    }

                    str = str.substr(tire_pos + distance);
                    tire_pos = str.find('-');
                }

                const auto stop_name = str.substr(0, tire_pos - 1);
                if (const Stop *stop = catalogue.GetStop(stop_name))
                {
                    bus.stops_.push_back(stop);
                }

                // Дублируем остановки для обратного маршрута
                const size_t size_ = bus.stops_.size() - 1;
                for (size_t i = size_; i > 0; --i)
                {
                    bus.stops_.push_back(bus.stops_[i - 1]);
                }
            }
            else
            {

                while (more_pos != std::string_view::npos)
                {
                    const auto stop_name = str.substr(0, more_pos - 1);
                    if (const Stop *stop = catalogue.GetStop(stop_name))
                    {
                        bus.stops_.push_back(stop);
                    }

                    str = str.substr(more_pos + distance);
                    more_pos = str.find('>');
                }

                const auto stop_name = str.substr(0, more_pos - 1);
                if (const Stop *stop = catalogue.GetStop(stop_name))
                {
                    bus.stops_.push_back(stop);
                }
            }

            return bus;
        }

        void input_(TransportCatalogue &catalogue)
        {
            std::string count;
            std::getline(std::cin, count);

            if (!count.empty())
            {
                std::string str;
                std::vector<std::string> buses;
                int amount = stoi(count);
                auto bus_distance = 3;

                for (int i = 0; i < amount; ++i)
                {
                    std::getline(std::cin, str);

                    if (!str.empty())
                    {
                        auto space_pos = str.find_first_not_of(' ');
                        str = str.substr(space_pos);

                        if (str.substr(0, bus_distance) != "Bus")
                        {

                            Stop stop = SplitStop(str);
                            catalogue.AddStop(stop.name_, stop.coordinates);
                        }
                        else
                        {

                            buses.push_back(str);
                        }
                    }
                }

                for (const auto &bus : buses)
                {
                    Bus bus_data = SplitBus(catalogue, bus);
                    std::vector<std::string_view> stop_names;
                    for (const auto &stop : bus_data.stops_)
                    {
                        stop_names.push_back(stop->name_);
                    }
                    catalogue.AddBus(bus_data.name_, stop_names);
                }
            }
        }
    }
}