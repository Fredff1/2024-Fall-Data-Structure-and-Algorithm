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

        // prev 用于存储前驱节点列表，以便追踪最短路径
        std::unordered_map<int, vector<int>> prev;
        for (int i = 0; i < graph.getVertices().size(); i++) {
            prev[i] = {};
        }

        // 使用优先队列来追踪最短距离的节点，最小堆
        std::priority_queue<distAndId, vector<distAndId>, std::greater<>> pq;
        pq.push({0, sourceId});

        auto adjList = graph.getAdjList();

        while (!pq.empty()) {
            auto [dist, u] = pq.top();
            pq.pop();

            // 如果队列中的距离大于当前记录的距离，跳过
            if (dist > distances[u]) {
                continue;
            }

            // 遍历邻接列表中的所有边
            for (const auto &edge : adjList[u]) {
                double newDist = distances[u] + edge.get()->getWeight();
                int v = edge->getSecond()->getId();

                // 找到更短路径时，更新距离和前驱节点
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    prev[v] = {u}; // 更新为新的前驱节点
                    pq.push({newDist, v});
                } else if (std::abs(newDist - distances[v]) < 1e-6) {
                    // 如果找到等距路径，添加新的前驱节点
                    if (std::find(prev[v].begin(), prev[v].end(), u) == prev[v].end()) {
                        prev[v].push_back(u); // 仅在前驱节点不存在的情况下添加
                    }
                }
            }
        }

        vector<vector<int>> allPaths;
        vector<int> path;

        // 回溯函数，用于生成所有路径
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

        // 从每个目标节点回溯路径
        for (int targetId = 0; targetId < graph.getVertices().size(); ++targetId) {
            if (distances[targetId] != INT_MAX) {
                backtrack(targetId);
            }
        }

        return allPaths;
    }
};

#endif
