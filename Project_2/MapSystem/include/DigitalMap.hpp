#ifndef DIGITAL_MAP_HPP
#define DIGITAL_MAP_HPP
#include "GraphAlgorithm.hpp"
#include "IOManager.hpp"
#include "Utilility.hpp"

#include <memory>

using std::unique_ptr,std::shared_ptr;



class MapNodeData {
  private:
    string name="";

    

  public:
    MapNodeData():name(""){
        
    }

    void setName(const string &name) {
        this->name = name;
    }

    const string &getName() const {
        return this->name;
    }
};

class DigitalMap {
    using VertexType = Vertex<MapNodeData>;
    using EdgeType = Edge<MapNodeData>;

  private:
    vector<shared_ptr<Edge<MapNodeData>>> edges;
    vector<shared_ptr<Vertex<MapNodeData>>> vertices;
    std::unordered_map<std::string, size_t> vertexMap;
    Graph<MapNodeData> mapGraph;
    vector<vector<double>> shortestPath;
    

    Graph<MapNodeData> initGraph(string path) {
        vertexMap = loadGraphFromFile(path);
        auto it = vertexMap.begin();
        while (it != vertexMap.end()) {
            auto &entry = (*it);
            auto id=entry.second;
            auto vertex=vertices[id];
            vertex.get()->getData().value().setName(entry.first);
            it++;
        }
        Graph<MapNodeData> graph = Graph<MapNodeData>(false, true, GraphRepresentationType::LIST_FORM, edges, vertices);
        return graph;
    }

    void initShortestPaths() {
        for (int i = 0; i < vertices.size(); i++) {
            auto newVec = vector<double>(vertices.size(), -100);
            newVec.resize(vertices.size());
            shortestPath.push_back(newVec);
        }
    }

  public:
    DigitalMap(string path = "./resources/edge.txt") : vertices(), edges(), mapGraph(initGraph(path)) {

    }

    Graph<MapNodeData> getGraph() const {
        return mapGraph;
    }

    void shortestPathFunc(string begin_name, string end_name) {
        VertexType* begin_vertex = vertices[vertexMap.at(begin_name)].get();
        VertexType* end_vertex = vertices[vertexMap.at(end_name)].get();

        GraphAlgorithms alg = GraphAlgorithms(mapGraph);
        auto shortestPaths=alg.dijkstra(begin_vertex->getId());
        for(const auto& path:shortestPaths){
            if(vertices[path.back()].get()->getData().value().getName()==end_vertex->getData().value().getName()){
                for(const auto& id:path){
                    VertexType* vtx=vertices[id].get();
                    auto name=vtx->getData().value().getName();
                    cout<<name<<" ";
                }
                cout<<endl;
            }
            
            
        }
    }

    std::unordered_map<std::string, size_t> loadGraphFromFile(
    const std::string& filename) {
        
    std::ifstream infile(filename);
   
    if (!infile.is_open()) {
        throw std::runtime_error("无法打开文件：" + filename);
    }

    std::string line;
    std::unordered_map<std::string, size_t> vertexMap;

    while (std::getline(infile, line)) {
        // 跳过空行或注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string node1, node2;
        double weight;

        if (!(iss >> node1 >> node2 >> weight)) {
            std::cerr << "Invalid line: " << line << std::endl;
            continue; // 跳过该行
        }

        // 创建顶点
        if (vertexMap.find(node1) == vertexMap.end()) {
            int index = vertices.size();
            vertexMap[node1] = index;
            MapNodeData data;
            data.setName(node1);
            auto vertex=std::make_shared<VertexType>(index,1,data);

            try{
                vertices.push_back(vertex);
            }catch(const std::exception &e){
                std::cerr << "Exception during vertex creation or push_back: " << e.what() << std::endl;
            }
             

        }

        if (vertexMap.find(node2) == vertexMap.end()) {
            int index = vertices.size();
            vertexMap[node2] = index;
            MapNodeData data;
            data.setName(node2);
            auto vertex=std::make_shared<VertexType>(index,1,data);
            vertices.push_back(vertex);
        }

        // 创建边
        size_t firstIndex = vertexMap[node1];
        size_t secondIndex = vertexMap[node2];
        edges.emplace_back(std::make_shared<EdgeType>(vertices[firstIndex], vertices[secondIndex], weight));
        edges.emplace_back(std::make_shared<EdgeType>(vertices[secondIndex], vertices[firstIndex], weight));
       
    }

    infile.close();
    
    return vertexMap;
}
};


#endif
