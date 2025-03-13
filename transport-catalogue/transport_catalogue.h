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
		geo::Coordinates coordinates;
	};

	struct Bus
	{
		std::string name_;
		std::vector<const Stop *> stops_;
	};

	using StopMap = std::unordered_map<std::string_view, const Stop *>;
	using BusMap = std::unordered_map<std::string_view, const Bus *>;
	using ConstBus = const Bus *;
	using ConstStop = const Stop *;

	class TransportCatalogue
	{
	public:
		void AddStop(std::string_view name, geo::Coordinates coordinates);
		void AddBus(std::string_view name, const std::vector<std::string_view> &stop_names);

		const Bus *GetBus(std::string_view bus_name) const;
		const Stop *GetStop(std::string_view stop_name) const;

		std::unordered_set<ConstBus> StopGetUniqBuses(const Stop *stop) const;
		std::unordered_set<ConstStop> GetUniqueStops(const Bus *bus) const;
		double GetLenght(const Bus *bus) const;

	private:
		std::deque<Stop> stops_;
		std::deque<Bus> buses_;
		StopMap stopname_to_stop;
		BusMap busname_to_bus;
		std::unordered_map<ConstStop, std::unordered_set<ConstBus>> stop_to_buses_;
	};
}