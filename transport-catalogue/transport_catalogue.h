#pragma once
#include <deque>
#include <string>
#include <vector>

#include <iomanip>
#include <iostream>

#include <unordered_set>
#include <unordered_map>

#include "geo.h"

namespace transport_catalogue
{
	struct Bus;

	struct Stop
	{
		std::string name_;
		double lat_;
		double lng_;

		std::vector<Bus*> buses_;
	};

	struct Bus
	{
		std::string name_;
		std::vector<Stop*> stops_;
	};

	typedef std::unordered_map<std::string_view, Stop*> StopMap;
	typedef std::unordered_map<std::string_view, Bus*> BusMap;

	class TransportCatalogue
	{
	public:
		void AddBus(Bus &&bus);
		void AddStop(Stop &&stop);

		Bus *GetBus(std::string_view bus_name);
		Stop *GetStop(std::string_view stop_name);

		std::unordered_set<const Bus*> StopGetUniqBuses(Stop *stop);
		std::unordered_set<const Stop*> GetUniqueStops(Bus *bus);
		double GetLenght(Bus *bus);

	private:
		std::deque<Stop> stops_;
		std::deque<Bus> buses_;
		StopMap stopname_to_stop;
		BusMap busname_to_bus;
	};
}