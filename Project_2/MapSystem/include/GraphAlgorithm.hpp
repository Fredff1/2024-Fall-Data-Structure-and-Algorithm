#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include "Graph.hpp"
#include <queue>
#include <limits>

template <typename DataType = std::monostate>
class GraphAlgorithms {
  private:
    const Graph<DataType>& graph; // 组合：使用现有的 Graph 类

  public:
    explicit GraphAlgorithms(const Graph<DataType>& g) : graph(g) {}

    // Dijkstra 算法：计算从源点到所有点的最短路径
    std::vector<int> dijkstra(int sourceId) const {
        const auto& adjList = graph.getAdjList();
        const int verticesNum = graph.getVerticesNum();

        // 距离数组，初始为无穷大
        std::vector<int> distances(verticesNum, std::numeric_limits<int>::max());
        distances[sourceId] = 0;

        // 优先队列 (min-heap)
        using PII = std::pair<int, int>; // {distance, vertexId}
        std::priority_queue<PII, std::vector<PII>, std::greater<>> pq;
        pq.emplace(0, sourceId);

        while (!pq.empty()) {
            auto [currentDist, currentId] = pq.top();
            pq.pop();

            if (currentDist > distances[currentId]) continue;

            // 遍历邻接点
            for (const auto& [neighbor, weight] : adjList[currentId]) {
                int neighborId = neighbor->getId();
                int newDist = currentDist + weight;

                if (newDist < distances[neighborId]) {
                    distances[neighborId] = newDist;
                    pq.emplace(newDist, neighborId);
                }
            }
        }

        return distances;
    }

    // Prim 算法：计算最小生成树的总权重
    int prim() const {
        const auto& adjList = graph.getAdjList();
        const int verticesNum = graph.getVerticesNum();

        // 是否已访问
        std::vector<bool> visited(verticesNum, false);
        // 最小堆 {weight, vertexId}
        using PII = std::pair<int, int>;
        std::priority_queue<PII, std::vector<PII>, std::greater<>> pq;

        // 开始节点假定为 0
        int totalWeight = 0;
        pq.emplace(0, 0);

        while (!pq.empty()) {
            auto [currentWeight, currentId] = pq.top();
            pq.pop();

            if (visited[currentId]) continue;
            visited[currentId] = true;
            totalWeight += currentWeight;

            // 遍历邻接点
            for (const auto& [neighbor, weight] : adjList[currentId]) {
                int neighborId = neighbor->getId();
                if (!visited[neighborId]) {
                    pq.emplace(weight, neighborId);
                }
            }
        }

        return totalWeight;
    }

    // Floyd-Warshall 算法：计算所有点对之间的最短路径
    std::vector<std::vector<int>> floydWarshall() const {
        auto adjMatrix = graph.getAdjMatrix();
        const int verticesNum = graph.getVerticesNum();

        // 初始化距离矩阵
        std::vector<std::vector<int>> distances = adjMatrix;
        for (int i = 0; i < verticesNum; ++i) {
            for (int j = 0; j < verticesNum; ++j) {
                if (i != j && distances[i][j] == 0) {
                    distances[i][j] = std::numeric_limits<int>::max(); // 无穷大
                }
            }
        }

        // 三重循环更新距离
        for (int k = 0; k < verticesNum; ++k) {
            for (int i = 0; i < verticesNum; ++i) {
                for (int j = 0; j < verticesNum; ++j) {
                    if (distances[i][k] != std::numeric_limits<int>::max() &&
                        distances[k][j] != std::numeric_limits<int>::max() &&
                        distances[i][k] + distances[k][j] < distances[i][j]) {
                        distances[i][j] = distances[i][k] + distances[k][j];
                    }
                }
            }
        }

        return distances;
    }
};

#endif 



