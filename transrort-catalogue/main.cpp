#include "json_reader.h"
#include "request_handler.h"

int main() {
    transport::Catalogue catalogue;
    JsonReader json_doc(std::cin, catalogue);
    
    json_doc.FillCatalogue();
    
    const auto& stat_requests = json_doc.GetStatRequests();
    const auto& render_settings = json_doc.GetRenderSettings();
    const auto renderer = json_doc.FillRenderSettings(render_settings);
    const auto router = json_doc.FillRoutingSettings(json_doc.GetRoutingSettings());
    
    RequestHandler rh(catalogue, renderer, router);
    json_doc.ProcessRequests(stat_requests, rh);
}