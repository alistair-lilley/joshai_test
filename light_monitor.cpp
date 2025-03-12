#ifndef LIGHT_MONITOR_CPP
#define LIGHT_MONITOR_CPP
#include "light_monitor.hpp"

LightMonitor::LightMonitor(std::string url, int p) 
{
    statusPageURL = url;
    port = p;
}

void LightMonitor::loopAndCheck()
{
    while (true) 
    {
        getAllLights(overAllLights);
        json oldLights = lightsStatus;
        checkEachLight(overAllLights, lightsStatus);
        std::vector<json> updates = getLightUpdate(oldLights, lightsStatus);
        if (!updates.empty())
        {
            printUpdates(updates);
        }
    }
}

void LightMonitor::getAllLights(json& status)
{
    httplib::Client cli(statusPageURL, port);
    
    if (auto res = cli.Get("/lights")) {
      if (res->status == 200) {
        try
        {
            status = json::parse(res->body);
        }
        catch (...)
        {
            std::cout << "Error parsing JSON" << std::endl;
        }
      }
    } else {
      auto err = res.error();
      std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    }
}

void LightMonitor::checkEachLight(json allLights, json& eachLight)
{
    httplib::Client cli(statusPageURL, port);
    json lights;
    for (json::iterator lightsit = allLights.begin(); lightsit != allLights.end(); ++lightsit)
    {
        std::string lightpath = "/lights/";
        lightpath = lightpath + (*lightsit)["id"].template get<std::string>();
        if (auto res = cli.Get(lightpath))
        {
            if (res->status == 200) {
                try
                {
                    lights.push_back(json::parse(res->body));
                }
                catch (...)
                {
                    std::cout << "Error parsing JSON" << std::endl;
                }
            }
        }
        else
        {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
    }
    eachLight = lights;
}

std::vector<json> LightMonitor::getLightUpdate(json oldStat, json newStat)
{
    std::vector<json> updates;
    for (json::iterator newit = newStat.begin(); newit != newStat.end(); ++newit)
    {
        bool found = false;
        for (json::iterator oldit = oldStat.begin(); oldit != oldStat.end(); ++oldit)
        {
            if ((*oldit)["id"] == (*newit)["id"])
            {
                found = true;
                if (*oldit != *newit)
                {
                    updates.push_back(getUpdateDiff(*newit, *oldit));
                }
            }
        }
        if (!found)
        {
            updates.push_back(*newit);
        }
    }
    for (json::iterator oldit = oldStat.begin(); oldit != oldStat.end(); ++oldit)
    {
        bool found = false;
        for (json::iterator newit = newStat.begin(); newit != newStat.end(); ++newit)
        {
            if ((*oldit)["id"] == (*newit)["id"])
            {
                found = true;
            }
        }
        if (!found)
        {
            *oldit += json::object_t::value_type("deleted", true);
            updates.push_back(*oldit);
        }
    }
    return updates;
}

json LightMonitor::getUpdateDiff(json newjson, json oldjson)
{
    json update_obj;
    for (auto newitem : newjson.items())
    {
        for (auto olditem : oldjson.items())
        {
            if (olditem.key() == newitem.key() && olditem.value() != newitem.value())
            {
                update_obj += json::object_t::value_type(newitem.key(), newitem.value());
            }
        }
    }
    update_obj += json::object_t::value_type("id", newjson["id"]);
    return update_obj;
}

void LightMonitor::printUpdates(std::vector<json> updates)
{
    for (auto item : updates)
    {
        if (item.contains("deleted"))
        {
            std::cout << item["name"] << " has been removed" << std::endl;
        }
        else
        {
            std::cout << item << std::endl;
        }
    }
}

#endif