#ifndef MODEL_HPP
#define MODEL_HPP

#include "DigitalMap.hpp"

class Model{
  private:
    DigitalMap map;
  public:
    Model(string path = "./resources/edge.txt"):map(){}

    vector<vector<int>> operation_I(string start_name,string end_name,string type="bellmanFord"){
        vector<vector<int>> result=map.shortestPathFunc(start_name,end_name,type);
        return result;
    }

    vector<vector<int>> operation_II(string start_name,string end_name,string type="dijkstra"){
        vector<vector<int>> result=map.shortestPathFunc(start_name,end_name,type);
        return result;
    }

    vector<shared_ptr<Edge<MapNodeData>>> operation_III(string start_name,string type){
        vector<shared_ptr<Edge<MapNodeData>>> result=map.MST(start_name);
        return result;
    }

    vector<vector<int>> operation_IV(string start_name,string type="dijkstra"){
        vector<vector<int>> result=map.shortestPathFunc(start_name,"A",type);
        return result;
    }

    DigitalMap& getMap(){
        return map;
    }

};



#endif 
