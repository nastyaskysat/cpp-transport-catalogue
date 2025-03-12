#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace transport_catalogue;
using namespace output;
using namespace input;

int main()
{
    TransportCatalogue tc;
    input_(tc);
    output_(tc);
}
// исправила catalogue на tc