#include "DM/JsonQuery.h"
#include <cstdarg>

namespace DM
{
    JsonQuery::JsonQuery(const Json& json)
        : _json(json)
    {}

    JsonQuery::Json JsonQuery::operator()(const std::string& path) const
    {
        return Json();
    }

    JsonQuery::Json JsonQuery::operator()(const std::string& path, 
                                          const std::vector<std::any>& params) const
    {
        
        return Json();
    }
}