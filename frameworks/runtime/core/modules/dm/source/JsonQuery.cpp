#include "DM/JsonQuery.h"

namespace DM
{
    JsonQuery::JsonQuery(const Json& json)
        : _json(json)
    {}

    JsonQuery::Json JsonQuery::operator()(const std::string& path) const
    {
        return Json();
    }

    JsonQuery::Json JsonQuery::operator()(const std::string& path, ...) const
    {
        return Json();
    }
}