#include "input_reader.h"
#include "stat_reader.h"

using namespace transport;

int main()
{
    TransportCatalogue tc;
    input_(std::cin, tc);
    output_(std::cin, tc, std::cout);
}