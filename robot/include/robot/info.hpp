#ifndef ROBOT_INFO_HPP
#define ROBOT_INFO_HPP

#if __APPLE__
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace robot {
struct Info {
    int id;
    std::pair<double, double> location;
    std::vector<int> station_plan;
    std::vector<int> waypoint_plan;
    double eta;

    Json::Value to_json() const;
    static Info from_json(const std::string &json);
    static Info from_json(const Json::Value &json);
};

struct InfoMap {
    std::map<int, Info> robot_info;

    InfoMap(){};
    InfoMap(const std::vector<Info> &infos);
    Json::Value to_json() const;
    Info &operator[](int index);
    const Info &operator[](int index) const;
    static InfoMap from_json(const std::string &json);
    static InfoMap from_json(const Json::Value &json);
};
} // namespace robot

#endif
