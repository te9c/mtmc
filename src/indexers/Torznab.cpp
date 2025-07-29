#include "Torznab.h"
#include "httplib.h"

std::string Torznab::SearchXml(std::string query) {
    auto res = client_.Get("/api?t=search&apikey=" + apikey_ + "&q=" + query);
}
