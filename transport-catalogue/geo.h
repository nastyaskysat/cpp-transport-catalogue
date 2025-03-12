#pragma once
#include <cmath>
namespace transport_catalogue
{
    namespace geo
    {
        struct Coordinates
        {
            double lat;
            double lng;

            bool operator==(const Coordinates &other) const
            {
                return lat == other.lat && lng == other.lng;
            }
        };

        inline double ComputeDistance(Coordinates start, Coordinates end)
        {
            using namespace std;
            if (!(start == end))
            {
                const double dr = 3.1415926535 / 180.;
                return acos(sin(start.lat * dr) * sin(end.lat * dr) + cos(start.lat * dr) * cos(end.lat * dr) * cos(abs(start.lng - end.lng) * dr)) * 6371000;
            }
            else
            {
                return 0.0;
            }
        }
    }
}
