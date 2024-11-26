#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include "Graph.hpp"
#include <limits>
#include <queue>

template <typename DataType = std::monostate>
class GraphAlgorithms {
  private:
    const Graph<DataType> &graph; // 组合：使用现有的 Graph 类

  public:
    explicit GraphAlgorithms(const Graph<DataType> &g) : graph(g) {
    }
    using distAndId = pair<double, int>;

    vector<vector<int>> dijkstra(int sourceId) {
        vector<double> distances(graph.getVertices().size(), INT_MAX);
        distances[sourceId] = 0;

        std::unordered_map<int, vector<int>> prev;
        for (int i = 0; i < graph.getVertices().size(); i++) {
            prev[i] = {};
        }

        std::priority_queue<distAndId, vector<distAndId>, std::greater<>> pq;
        pq.push({0, sourceId});

        auto adjList = graph.getAdjList();

        while (!pq.empty()) {
            auto [dist, u] = pq.top();
            pq.pop();

            if (dist > distances[u]) {
                continue;
            }

            for (const auto &edge : adjList[u]) {
                double newDist = distances[u] + edge.get()->getWeight();

                int v = edge.get()->getSecond()->getId();

                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    prev[v] = {u};
                    pq.push({newDist, v});
                } else if (std::abs(newDist - distances[v]) < 1e-6) {

                    if (std::find(prev[v].begin(), prev[v].end(), u) == prev[v].end()) {
                        prev[v].push_back(u);
                    }
                }
            }
        }

        vector<vector<int>> allPaths;
        vector<int> path;

        std::function<void(int)> backtrack = [&](int node) {
            if (node == sourceId) {
                path.push_back(node);
                allPaths.push_back(vector<int>(path.rbegin(), path.rend())); // 反转存储路径
                path.pop_back();
                return;
            }
            path.push_back(node);
            for (int p : prev[node]) {
                backtrack(p);
            }
            path.pop_back();
        };

        for (int targetId = 0; targetId < graph.getVertices().size(); ++targetId) {
            if (distances[targetId] != INT_MAX) {
                backtrack(targetId);
            }
        }

        return allPaths;
    }

    vector<vector<int>> bellmanFord(int sourceId) {
        int n = graph.getVertices().size();
        vector<double> distances(n, INT_MAX);
        distances[sourceId] = 0;

        // 记录前驱节点，用于回溯路径
        std::unordered_map<int, vector<int>> prev;
        for (int i = 0; i < n; ++i) {
            prev[i] = {};
        }

        vector<std::shared_ptr<Edge<DataType>>> edges = graph.getEdges(); // 获取图中所有边
        bool updated;
        for (int i = 1; i < n; i++) {
            updated = false;
            for (const std::shared_ptr<Edge<DataType>> &edge_ptr : edges) {
                Edge<DataType> edge = (*edge_ptr.get());
                int u = edge.getFirst().get()->getId();
                int v = edge.getSecond().get()->getId();
                double weight = edge.getWeight();
                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    prev[v] = {u};
                    updated = true;
                } else if (std::abs(distances[u] + weight - distances[v]) < 1e-6) {

                    if (std::find(prev[v].begin(), prev[v].end(), u) == prev[v].end()) {
                        prev[v].push_back(u);
                    }
                }
            }
            if (!updated) {
                break;
            }
        }
        vector<vector<int>> allPaths;
        vector<int> path;

        std::function<void(int)> backtrack = [&](int node) {
            if (node == sourceId) {
                path.push_back(node);
                allPaths.push_back(vector<int>(path.rbegin(), path.rend())); // 反转存储路径
                path.pop_back();
                return;
            }
            path.push_back(node);
            for (int p : prev[node]) {
                backtrack(p);
            }
            path.pop_back();
        };

        for (int targetId = 0; targetId < graph.getVertices().size(); ++targetId) {
            if (distances[targetId] != INT_MAX) {
                backtrack(targetId);
            }
        }

        return allPaths;
    }

    vector<shared_ptr<Edge<DataType>>> prim(int startId) {
        int n = graph.getVertices().size();
        vector<bool> InMST(n, false);
        std::priority_queue<shared_ptr<Edge<DataType>>, vector<shared_ptr<Edge<DataType>>>, 
                   std::function<bool(const shared_ptr<Edge<DataType>>&, const shared_ptr<Edge<DataType>>&)>> 
        pq([](const shared_ptr<Edge<DataType>>& a, const shared_ptr<Edge<DataType>>& b) {
            return a->getWeight() > b->getWeight(); // 按边权从小到大排序
        });
        vector<shared_ptr<Edge<DataType>>> mstEdges; // 存储生成树的边
        InMST[startId] = true;
        auto adjList = graph.getAdjList();
        for (const auto &edge_ptr : adjList[startId]) {
            pq.push(edge_ptr);
        }
        while (!pq.empty()) {
            auto edge_ptr = pq.top();
            pq.pop();

            Edge<DataType>* edge = edge_ptr.get();
            int dest = edge->getSecond()->getId(); // 边的目标节点

      
            if (InMST[dest]) continue;

            mstEdges.push_back(edge_ptr);

            InMST[dest] = true; 

      
            for (const auto &edge : adjList[dest]) {
                int next = edge.get()->getSecond()->getId();
                if (!InMST[next]) {
                    pq.push(edge);
                }
            }

        }
        return mstEdges;
    }
};

#endif
