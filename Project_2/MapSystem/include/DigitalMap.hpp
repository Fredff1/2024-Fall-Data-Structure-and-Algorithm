#ifndef DIGITAL_MAP_HPP
#define DIGITAL_MAP_HPP
#include "IOManager.hpp"

class DigitalMap{
  private:
    Graph<> mapGraph;

    Graph<> initGraph(string path){
        vector<Edge<>> edges;
        vector<Vertex<>> vertices;
        IOManager<>::loadGraphFromFile(path,vertices,edges);
        Graph<> graph=Graph<>(false,true,GraphRepresentationType::LIST_FORM,edges,vertices);
        return graph;
    }

  public:
    DigitalMap(string path="./resources/edge.txt"):mapGraph(initGraph(path)){

    }

    Graph<> getGraph() const{
        return mapGraph;
    }
};


#endif 
