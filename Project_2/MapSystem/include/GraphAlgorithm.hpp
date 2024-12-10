#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include "GlobalFlags.hpp"
#include "Graph.hpp"

#include <any>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>

template <typename DataType = std::monostate>
class GraphAlgorithms {
  private:
    const Graph<DataType> &graph;


    mutable std::unordered_map<std::string, std::unordered_map<int, std::any>> cache;


    std::string generateCacheKey(const std::string &algoName, int id) const {
        return algoName + "_" + std::to_string(id);
    }


    template <typename T>
    std::optional<T> getCachedResult(const std::string &key, int id) const {
        auto it = cache.find(key);
        if (it != cache.end()) {
            auto innerMap = it->second;
            if (innerMap.find(id) != innerMap.end()) {
                return std::any_cast<T>(innerMap.at(id));
            }
        }
        return std::nullopt;
    }


    template <typename T>
    void storeInCache(const std::string &key, int id, const T &result) const {
        cache[key][id] = result;
    }

    double calculateTotalWeight(const std::vector<std::shared_ptr<Edge<DataType>>> &edges) const {
        double total = 0;
        for (const auto &edge : edges) {
            total += edge.get()->getWeight();
        }
        return total;
    }

  public:
    explicit GraphAlgorithms(const Graph<DataType> &g) : graph(g) {
    }
    using distAndId = pair<double, int>;

    vector<vector<int>> dijkstra(int sourceId) {
        std::string key = "dijkstra";
        if (auto cachedResult = getCachedResult<std::vector<std::vector<int>>>(key, sourceId)) {
            return *cachedResult;
        }
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
        storeInCache(key, sourceId, allPaths);
        return allPaths;
    }

    vector<vector<int>> bellmanFord(int sourceId) {
        std::string key = "bellmanFord";
        if (auto cachedResult = getCachedResult<std::vector<std::vector<int>>>(key, sourceId)) {
            return *cachedResult;
        }
        int n = graph.getVertices().size();
        vector<double> distances(n, INT_MAX);
        distances[sourceId] = 0;


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
        storeInCache(key, sourceId, allPaths);
        return allPaths;
    }

    std::vector<std::shared_ptr<Edge<DataType>>> prim(int startId) {
        string key = "prim";
        if (auto cachedResult = getCachedResult<vector<shared_ptr<Edge<DataType>>>>(key, startId)) {
            return *cachedResult;
        }

        int n = graph.getVertices().size();
        vector<bool> InMST(n, false);
        std::priority_queue<shared_ptr<Edge<DataType>>, vector<shared_ptr<Edge<DataType>>>, std::function<bool(const shared_ptr<Edge<DataType>> &, const shared_ptr<Edge<DataType>> &)>> pq([](const shared_ptr<Edge<DataType>> &a, const shared_ptr<Edge<DataType>> &b) {
            return a->getWeight() > b->getWeight(); 
        });
        vector<shared_ptr<Edge<DataType>>> mstEdges; 
        InMST[startId] = true;
        auto adjList = graph.getAdjList();
        for (const auto &edge_ptr : adjList[startId]) {
            pq.push(edge_ptr);
        }
        while (!pq.empty()) {
            auto edge_ptr = pq.top();
            pq.pop();

            Edge<DataType> *edge = edge_ptr.get();
            int dest = edge->getSecond()->getId(); 

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
        storeInCache(key, startId, mstEdges);
        return mstEdges;
    }

      

    // // Kruskal算法
    // vector<shared_ptr<Edge<DataType>>> kruskal() {
    //     int n = graph.getVertices().size();
    //     std::vector<shared_ptr<Edge<DataType>>> edges = graph.getEdges();
    //     std::vector<shared_ptr<Edge<DataType>>> mstEdges;

    //     
    //     std::vector<int> parent(n);
    //     std::vector<int> rank(n, 0);

    //     
    //     for (int i = 0; i < n; ++i) {
    //         parent[i] = i;
    //     }

    //     auto find = std::function<int(int)>([&](int x) {
    //         if (parent[x] != x) {
    //             parent[x] = find(parent[x]); 
    //         }
    //         return parent[x];
    //     });

    //     
    //     auto unionSets = [&](int x, int y) {
    //         int rootX = find(x);
    //         int rootY = find(y);
    //         if (rootX != rootY) {
    //             if (rank[rootX] > rank[rootY]) {
    //                 parent[rootY] = rootX;
    //             } else if (rank[rootX] < rank[rootY]) {
    //                 parent[rootX] = rootY;
    //             } else {
    //                 parent[rootY] = rootX;
    //                 rank[rootX]++;
    //             }
    //             return true;
    //         }
    //         return false;
    //     };

    //     
    //     std::sort(edges.begin(), edges.end(), [](const shared_ptr<Edge<DataType>> &a, const shared_ptr<Edge<DataType>> &b) { return a->getWeight() < b->getWeight(); });

    //     
    //     for (const auto &edge : edges) {
    //         int u = edge->getFirst()->getId();
    //         int v = edge->getSecond()->getId();
    //         if (unionSets(u, v)) { 
    //             mstEdges.push_back(edge);
    //         }
    //         if (mstEdges.size() == n - 1) break; 
    //     }

    //     return mstEdges;
    // }
    double calculateMinimumTotalWeight(const std::vector<std::shared_ptr<Edge<DataType>>> &edges, int n) {
        std::vector<int> parent(n);
        std::vector<int> rank(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }

        auto find = [&](int x, std::vector<int> &parentRef) -> int {
            while (x != parentRef[x]) {
                parentRef[x] = parentRef[parentRef[x]]; 
                x = parentRef[x];
            }
            return x;
        };

        double totalWeight = 0.0;
        for (const auto &edge : edges) {
            int u = edge->getFirst()->getId();
            int v = edge->getSecond()->getId();
            double weight = edge->getWeight();
            int rootU = find(u, parent);
            int rootV = find(v, parent);
            if (rootU != rootV) {
                parent[rootU] = rootV;
                totalWeight += weight;
            }
        }
        return totalWeight;
    }

    std::vector<std::vector<std::shared_ptr<Edge<DataType>>>> kruskal() {
        string key = "kruskal";
        if (auto cachedResult = getCachedResult<vector<std::vector<std::shared_ptr<Edge<DataType>>>>>(key, 0)) {
            return *cachedResult;
        }
        int n = graph.getVertices().size();
        std::vector<std::shared_ptr<Edge<DataType>>> edges = graph.getEdges();
        std::vector<std::vector<std::shared_ptr<Edge<DataType>>>> results;

        std::set<std::set<std::pair<int, int>>> uniqueMSTs; 

        std::sort(edges.begin(), edges.end(), [](const std::shared_ptr<Edge<DataType>> &a, const std::shared_ptr<Edge<DataType>> &b) { return a->getWeight() < b->getWeight(); });

        
        double minTotalWeight = calculateMinimumTotalWeight(edges, n);

      
        std::function<void(int, std::vector<std::shared_ptr<Edge<DataType>>>, std::vector<int>, std::vector<int>, double)> backtrack;
        backtrack = [&](int edgeIndex, std::vector<std::shared_ptr<Edge<DataType>>> currentMST, std::vector<int> parent, std::vector<int> rank, double currentWeight) {
           
            if(results.size()>=2){
                return;
            }

            
            if (currentMST.size() == n - 1) {
               
                if (std::abs(currentWeight - minTotalWeight) < 1e-6) {
                    std::set<std::pair<int, int>> edgeSet;
                    for (const auto &edge : currentMST) {
                        int u = edge->getFirst()->getId();
                        int v = edge->getSecond()->getId();
                        if (u > v) std::swap(u, v);
                        edgeSet.insert({u, v});
                    }
                    if (uniqueMSTs.find(edgeSet) == uniqueMSTs.end()) {
                        uniqueMSTs.insert(edgeSet);
                        results.push_back(currentMST);
                    }
                }
                return;
            }

            
            if (edgeIndex >= edges.size() || currentMST.size() + (edges.size() - edgeIndex) < n - 1 || currentWeight > minTotalWeight) {
                return;
            }

            for (int i = edgeIndex; i < edges.size(); ++i) {
                const auto &currentEdge = edges[i];
                int u = currentEdge->getFirst()->getId();
                int v = currentEdge->getSecond()->getId();
                double weight = currentEdge->getWeight();

               
                auto find = [&](int x, std::vector<int> &parentRef) -> int {
                    while (x != parentRef[x]) {
                        parentRef[x] = parentRef[parentRef[x]]; 
                        x = parentRef[x];
                    }
                    return x;
                };

                int rootU = find(u, parent);
                int rootV = find(v, parent);

                if (rootU != rootV) {
                    
                    std::vector<std::shared_ptr<Edge<DataType>>> newMST = currentMST;
                    newMST.push_back(currentEdge);
                    double newWeight = currentWeight + weight;

                   
                    std::vector<int> newParent = parent;
                    std::vector<int> newRank = rank;

                  
                    if (newRank[rootU] < newRank[rootV]) {
                        newParent[rootU] = rootV;
                    } else if (newRank[rootU] > newRank[rootV]) {
                        newParent[rootV] = rootU;
                    } else {
                        newParent[rootV] = rootU;
                        newRank[rootU]++;
                    }

                    
                    backtrack(i + 1, newMST, newParent, newRank, newWeight);
                }
                
                if (i < edges.size() - 1 && std::abs((edges[i].get()->getWeight() - edges[i + 1].get()->getWeight() ))< 1e-6) {
                    int id_1=edges[i]->getSecond().get()->getId();
                    int id_2=edges[i+1]->getFirst().get()->getId();
                    bool flag_1=id_1!=id_2;
                    if(flag_1){
                        backtrack(i + 1, currentMST, parent, rank, currentWeight);
                    }
                    
                }
            }
        };

        
        std::vector<int> initialParent(n);
        std::vector<int> initialRank(n, 0);
        for (int i = 0; i < n; ++i) {
            initialParent[i] = i;
        }

        std::vector<std::shared_ptr<Edge<DataType>>> currentMST;
        double initialWeight = 0.0;
        backtrack(0, currentMST, initialParent, initialRank, initialWeight);
        storeInCache(key, 0, results);
        return results;
    }
};

#endif
