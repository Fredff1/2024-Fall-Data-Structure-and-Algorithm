#ifndef MODEL_HPP
#define MODEL_HPP

#include "DigitalMap.hpp"
#include "GlobalFlags.hpp"

class Model {
  private:
    DigitalMap map;

  public:
    Model(string path = "./resources/edge.txt") : map() {
    }

    string operation_I(string start_name, string end_name, string type = "bellmanFord") {
        vector<vector<int>> result = map.shortestPathFunc(start_name, end_name, type);
        string content = map.shortestPathOneToString(result, end_name);
        return "Algorithm: " + type + "\n" + content;
    }

    string operation_II(string start_name, string type = "dijkstra") {
        vector<vector<int>> result = map.shortestPathFunc(start_name, "A", type);
        string content = map.shortestPathsToString(result);
        return "Algorithm: " + type + "\n" + content;
    }

    string operation_III(string start_name = "A", string type = "prim") {
        auto result = map.MST(start_name, type);
        string content = map.mstToString(result);
        return "Algorithm: " + type + "\n" + content;
    }

    string operation_IV(string start_name, string type = "dijkstra") {
        vector<vector<int>> result = map.shortestPathFunc(start_name, "A", type);
        string content = map.shortestPathsToStringSingleStyle(result);
        return "Algorithm: " + type + "\n" + content;
    }

    DigitalMap &getMap() {
        return map;
    }
};

#endif
