#pragma once

#include <nlohmann/json.hpp>

namespace DM
{
    // Accessing properties: person.name
    // Array accessors: people[0]
    // Array pluck: people.name => return all the names of people
    // Get all values of a lookup: lookup[*]
    // Array filter: people[name=Matt]  (By default only the first matching item will be returned)
    //               people[*name=Matt] (But if you add an asterisk (*), all matching items will be returned)
    // Comparative operators: people[*rating>=3]
    // Boolean logic: people[* rating >= 3 & starred = true]
    // Deep queries: grouped_people[**][*country=NZ] (Search through multiple levels of Objects/Arrays using [**])
    // Inner queries: comments_lookup[{page.id}] (page is in the same json with comments_loopup)
    // Query params: ('people[country=?]', 'NZ')
    // Predicate: ('?(people)', func) 
    class JsonQuery
    {
    public:
        using Json = nlohmann::json;
        using PredFunc = std::function<bool(const Json&)>;

        JsonQuery() = default;
        JsonQuery(const Json& json);

        Json operator()(const std::string& path) const;
        Json operator()(const std::string& path, ...) const; // QueryParams or Predicate

    private:
        Json _json;
    }; 
}