#ifndef LIGHT_MONITOR_HPP
#define LIGHT_MONITOR_HPP
#include <iostream>
#include <vector>
#include <string.h>
#include <httplib.h>
#include <string_view>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class LightMonitor {
private:
    json overAllLights;
    json lightsStatus;
    std::string statusPageURL;
    int port;
public:
    LightMonitor(std::string, int);
    void loopAndCheck();
    void getAllLights(json&);
    void checkEachLight(json, json&);
    std::vector<json> getLightUpdate(json, json);
    json getUpdateDiff(json, json);
    void printUpdates(std::vector<json>);
};

#endif