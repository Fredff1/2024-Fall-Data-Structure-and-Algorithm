#ifndef FLOW_HPP
#define FLOW_HPP
#include "Graph.hpp"

#include <fstream>
#include <queue>
#include <sstream>


using std::shared_ptr;

struct EdgeCapacity {
    double capacity = 0;
};

struct FlowResult {
    double maxFlow;
    double minCost;
};

class GraphFlow {
  private:
    vector<shared_ptr<Edge<EdgeCapacity>>> edges;
    vector<shared_ptr<Vertex<EdgeCapacity>>> vertices;
    Graph<EdgeCapacity> graph;
    int source;
    int sink;

  public:
    GraphFlow(string &path) : graph(GraphFlow::loadFromFile(path, edges, vertices)) {
    }

    Graph<EdgeCapacity> loadFromFile(const string &path, vector<shared_ptr<Edge<EdgeCapacity>>> &edges, vector<shared_ptr<Vertex<EdgeCapacity>>> &vertices) {
        std::ifstream infile(path);

        if (!infile.is_open()) {
            throw std::runtime_error("无法打开文件：" + path);
        }

        std::string line;
        std::unordered_map<int, size_t> vertexMap;

        int n, m, s, t;
        if (!std::getline(infile, line) || line.empty()) {
            throw std::runtime_error("文件格式错误：缺少图的基本信息");
        }
        std::istringstream firstLine(line);
        if (!(firstLine >> n >> m >> s >> t)) {
            throw std::runtime_error("文件格式错误：图的基本信息解析失败");
        }

        source = s - 1;
        sink = t - 1;

        for (int i = 1; i <= n; ++i) {
            auto vertex = std::make_shared<Vertex<EdgeCapacity>>(i - 1, 1);
            vertices.push_back(vertex);
            vertexMap[i] = i - 1;
        }

        while (std::getline(infile, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            int node1, node2;
            double capacity, cost;

            if (!(iss >> node1 >> node2 >> capacity >> cost)) {
                std::cerr << "无效的行：" << line << std::endl;
                continue;
            }

            if (vertexMap.find(node1) == vertexMap.end() || vertexMap.find(node2) == vertexMap.end()) {
                std::cerr << "无效的节点编号：" << line << std::endl;
                continue;
            }

            size_t firstIndex = vertexMap[node1];
            size_t secondIndex = vertexMap[node2];

            auto new_edge = std::make_shared<Edge<EdgeCapacity>>(vertices[firstIndex], vertices[secondIndex], cost, true);
            new_edge->setCapacity(capacity);
            new_edge->setResidualEdge(std::make_shared<Edge<EdgeCapacity>>(vertices[secondIndex], vertices[firstIndex], 0, true));
            edges.emplace_back(new_edge);
        }

        infile.close();

        Graph<EdgeCapacity> graph = Graph<EdgeCapacity>(true, true, GraphRepresentationType::LIST_FORM, edges, vertices);
        return graph;
    }

    FlowResult minCostMaxFlow() {
        return minCostMaxFlowFunc(source, sink);
    }

    FlowResult minCostMaxFlow(int source, int sink) {
        return minCostMaxFlowFunc(source, sink);
    }

    FlowResult minCostMaxFlowFunc(int source, int sink) {
        double maxFlow = 0.0;
        double minCost = 0.0;

        while (true) {
            /* Step I :Shortest Path*/
            vector<double> dist(vertices.size(), std::numeric_limits<double>::infinity());
            vector<shared_ptr<Edge<EdgeCapacity>>> prevEdge(vertices.size(), nullptr);
            vector<bool> inQueue(vertices.size(), false);

            dist[source] = 0.0;
            std::queue<int> q;
            q.push(source);
            inQueue[source] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inQueue[u] = false;

                for (const auto &edge : graph.getAdjList()[u]) {
                    if (edge->getResidualCapacity() > 0) { 
                        int v = edge->getSecond()->getId();
                        double newDist = dist[u] + edge->getWeight(); 
                        if (newDist < dist[v]) {
                            dist[v] = newDist;
                            prevEdge[v] = edge;

                            if (!inQueue[v]) {
                                q.push(v);
                                inQueue[v] = true;
                            }
                        }
                    }
                }
            }

            /* Not reachable*/
            if (dist[sink] == std::numeric_limits<double>::infinity()) {
                break;
            }

            /* Step II Calculate augmentFlow ,the minium residual capacity in the path*/
            double augmentFlow = std::numeric_limits<double>::infinity();
            for (int v = sink; v != source; v = prevEdge[v]->getFirst()->getId()) {
                augmentFlow = std::min(augmentFlow, prevEdge[v]->getResidualCapacity());
            }

            // Step 3: Update maxFlow and miniCost
            for (int v = sink; v != source; v = prevEdge[v]->getFirst()->getId()) {
                auto edge = prevEdge[v];
                edge->addFlow(augmentFlow);                     
                minCost += augmentFlow * edge->getWeight();     
            }

            maxFlow += augmentFlow; 
        }

        return {maxFlow, minCost};
    }
};

#endif
