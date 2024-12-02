#ifndef DIGITAL_MAP_HPP
#define DIGITAL_MAP_HPP
#include "GraphAlgorithm.hpp"
#include "IOManager.hpp"
#include "Utilility.hpp"
#include "GlobalFlags.hpp"
#include <set>

#include <memory>

using std::unique_ptr, std::shared_ptr;

class MapNodeData {
  private:
    string name = "";

  public:
    MapNodeData() : name("") {
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
    GraphAlgorithms<MapNodeData> alg;
    vector<vector<double>> shortestPath;

    Graph<MapNodeData> initGraph(string path) {
        vertexMap = loadGraphFromFile(path);
        auto it = vertexMap.begin();
        while (it != vertexMap.end()) {
            auto &entry = (*it);
            auto id = entry.second;
            auto vertex = vertices[id];
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
    DigitalMap(string path = "./resources/edge.txt") : vertices(), edges(), mapGraph(initGraph(path)),alg(GraphAlgorithms(mapGraph)) {
    }

    Graph<MapNodeData> getGraph() const {
        return mapGraph;
    }

    bool validateName(string name) const{
        if(vertexMap.find(name)==vertexMap.end()){
            return false;
        }else{
            return true;
        }
    }

    std::vector<std::vector<int>> shortestPathFunc(string begin_name, string end_name, string algorithm_type = "dijkstra") {
        VertexType *begin_vertex = vertices[vertexMap.at(begin_name)].get();
        VertexType *end_vertex = vertices[vertexMap.at(end_name)].get();

        std::vector<std::vector<int>> shortestPaths;
        if (algorithm_type == "dijkstra") {
            shortestPaths = alg.dijkstra(begin_vertex->getId());
        } else if (algorithm_type == "bellmanFord") {
            shortestPaths = alg.bellmanFord(begin_vertex->getId());
        }

        #ifdef DEBUG_GRAPH
        cout<<shortestPathsToString(shortestPaths);
        #endif
        
        return shortestPaths;
    }

    vector<shared_ptr<Edge<MapNodeData>>> MST(string begin_name="A",string type="prim") {
        VertexType *begin_vertex = vertices[vertexMap.at(begin_name)].get();
        
        vector<shared_ptr<Edge<MapNodeData>>> mstResult;
        if(type=="prim"){
           // mstResult = alg.prim(begin_vertex->getId());
        }else if(type=="kruskal"){
            mstResult=alg.kruskal().at(0);
        }
        


        #ifdef DEBUG_GRAPH
        try{
            auto results=alg.kruskal();
            for(const auto&single:results){
                cout<<mstToString(single);
                cout<<"-------------------------------\n";
            }
           
            //cout<<mstToString(mstResult);
        }catch(std::exception&e){

        }
            
        #endif
        return mstResult;
    }

    std::string shortestPathOneToString(const std::vector<std::vector<int>> &shortestPaths,string end_name) {
        VertexType *end_vertex = vertices[vertexMap.at(end_name)].get();
        std::ostringstream oss;
        double full_len=0;
        oss << "Shortest Path:\n";
        for (const auto &path : shortestPaths) {
            if (vertices[path.back()].get()->getData().value().getName() == end_vertex->getData().value().getName()){
                oss << "[ ";
            for (size_t i = 0; i < path.size(); ++i) {
                oss << vertices[path[i]]->getData().value().getName();
                if (i != path.size() - 1) {
                    oss << " -> ";
                    full_len+=mapGraph.getEdge(path[i],path[i+1]).get()->getWeight();
                }
                
            }
            
            oss << " length:"<<full_len<<"km ]\n";
            full_len=0;
            }
            
        }
        return oss.str();
    }

    std::string shortestPathsToString(const std::vector<std::vector<int>> &shortestPaths) {
        std::ostringstream oss;
        oss << "Shortest Paths:\n";
        double full_len=0;
        for (const auto &path : shortestPaths) {
            oss << "[ ";
            for (size_t i = 0; i < path.size(); ++i) {
                oss << vertices[path[i]]->getData().value().getName();
                if (i != path.size() - 1) {
                    oss << " -> ";
                    full_len+=mapGraph.getEdge(path[i],path[i+1]).get()->getWeight();
                }
                
            }
            oss << " length:"<<full_len<<"km ]\n";
            full_len=0;
        }
        return oss.str();
    }

    std::string shortestPathsToStringSingleStyle(const std::vector<std::vector<int>> &shortestPaths) {
        std::ostringstream oss;
        oss << "Shortest Paths (Edges):\n";
        double full_len=0;
        // 使用 set 来去重
        std::set<std::pair<std::string, std::string>> uniqueEdges;

        for (const auto &path : shortestPaths) {
            for (size_t i = 0; i < path.size() - 1; ++i) {
                // 获取边的起点和终点
                std::string from = vertices[path[i]]->getData().value().getName();
                std::string to = vertices[path[i + 1]]->getData().value().getName();
                double weight=mapGraph.getEdge(path[i],path[i+1]).get()->getWeight();
                full_len+=weight;

                // 保证无重复输出
                if (uniqueEdges.emplace(from, to).second) {  // emplace 返回 true 表示插入成功
                    oss << from << " -> " << to << " length:"<<weight<<"km\n";
                }
            }
            // oss<<"Total:"<<full_len<<"km\n";
            // full_len=0;
        }
        
        return oss.str();
    }

    std::string mstToString(const std::vector<std::shared_ptr<Edge<MapNodeData>>> &mstResult,string prefix="") {
        std::ostringstream oss;
        double full_len=0;
        oss << "Minimum Spanning Tree (MST):\n";
        for (const auto &edge_ptr : mstResult) {
            Edge<MapNodeData> *edge = edge_ptr.get();
            auto from = edge->getFirst()->getData().value().getName();
            auto to = edge->getSecond()->getData().value().getName();
            double weight = edge->getWeight();
            full_len+=weight;
            oss << "Edge: " << from << " -> " << to << ", Length: " << weight << "km\n";
        }
        oss<<"Total length: "<<full_len<<"\n";
        return prefix+oss.str();
    }

    std::unordered_map<std::string, size_t> loadGraphFromFile(const std::string &filename) {

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
                auto vertex = std::make_shared<VertexType>(index, 1, data);

                try {
                    vertices.push_back(vertex);
                } catch (const std::exception &e) {
                    std::cerr << "Exception during vertex creation or push_back: " << e.what() << std::endl;
                }
            }

            if (vertexMap.find(node2) == vertexMap.end()) {
                int index = vertices.size();
                vertexMap[node2] = index;
                MapNodeData data;
                data.setName(node2);
                auto vertex = std::make_shared<VertexType>(index, 1, data);
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
