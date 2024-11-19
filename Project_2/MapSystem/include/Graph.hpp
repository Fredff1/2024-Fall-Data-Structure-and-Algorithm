#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Utilility.hpp"

#include <list>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

using std::list;
using std::pair;
using std::tuple;

enum class GraphRepresentationType { LIST_FORM, MATRIX_FORM };

template <typename DataType = std::monostate>
class Vertex {
  private:
    int id;
    int weight; 
    bool hasWeight = false;
    std::optional<DataType> data; 

  public:
    Vertex() : id(-1), weight(0) {
    }

    Vertex(int id) : id(id), weight(0) {
    }

    Vertex(int id, int weight) : id(id), weight(weight), hasWeight(true) {
    }

    Vertex(int id, int weight, DataType& data) : id(id), weight(weight), hasWeight(true), data(data) {
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    int getWeight() const {
        return weight;
    }

    void setWeight(int newWeight) {
        weight = newWeight;
    }

    std::optional<DataType> getData() const {
        return data;
    }

    void setData(const DataType &newData) {
        data = newData;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vertex &vertex) {
        os << "Vertex(ID: " << vertex.id;
        if (vertex.hasWeight) {
            cout << ", vertex weight: " << vertex.weight;
        }
        os << ")";
        return os;
    }

    template <typename U>
    Vertex(const Vertex<U> &other) : id(other.getId()), weight(other.getWeight()) {
    }
};

template <typename DataType = std::monostate>
class Edge {
  private:
    bool isDirect = false;
    Vertex<DataType> *first;
    Vertex<DataType> *second;
    double weight;

  public:

    Edge(Vertex<DataType> &first, Vertex<DataType> &second, double weight, bool isDirect) : first(&first), second(&second), weight(weight), isDirect(isDirect) {
    }

    Edge(Vertex<DataType> &first, Vertex<DataType> &second, double weight) : first(&first), second(&second), weight(weight), isDirect(false) {
    }

    Edge(Vertex<DataType> &first, Vertex<DataType> &second, bool isDirect) : first(&first), second(&second), weight(1), isDirect(isDirect) {
    }

    Edge(Vertex<DataType> &first, Vertex<DataType> &second) : first(&first), second(&second), weight(1), isDirect(false) {
    }


    Vertex<DataType> *getFirst() const {
        return first;
    }


    Vertex<DataType> *getSecond() const {
        return second;
    }


    double getWeight() const {
        return weight;
    }


    bool getIsDirect() const {
        return isDirect;
    }


    void setWeight(double newWeight) {
        weight = newWeight;
    }


    void setIsDirect(bool direction) {
        isDirect = direction;
    }

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
        os << "Edge(" << edge.first.id << " -> " << edge.second.id << ", Weight: " << edge.weight << ", Directed: " << (edge.isDirect ? "Yes" : "No") << ")";
        return os;
    }
};

template <typename DataType = std::monostate>
class Graph {
    using EdgeType = Edge<DataType>;
    using VertexType = Vertex<DataType>;

  private:
    int verticesNum; // num of nodes
    bool isDirect;   // has direction
    GraphRepresentationType rep;

    vector<VertexType> vertexVec;
    vector<list<pair<VertexType *, double>>> adjList; // Adjacency List
    vector<std::vector<double>> adjMatrix;            // Adjacency Matrix

    bool hasAdjList = false;
    bool hasAdjMatrix = false;
    bool hasEdgeWeight = false;

    void dfsHelper(VertexType* v,vector<bool>& visited,vector<int>& result){
        visited[v->getId()]=true;
        for(const pair<VertexType *, int>&target:adjList[v->getId()]){
            VertexType * neighbor=target.first;
            int weight=target.second;
            if(!visited[neighbor->getId()]){
                dfsHelper(neighbor,visited,result);
            }
        }
        result.push_back(v->getId());
    }

    bool dfsUtilForCycle(VertexType* v, std::vector<bool>& visited, std::vector<bool>& recStack) {
        int id = v->getId();

        visited[id] = true;   
        recStack[id] = true;  


        for (const auto& [neighbor, weight] : adjList[id]) {
            int neighborId = neighbor->getId();

            if (!visited[neighborId]) {
                if (dfsUtilForCycle(neighbor, visited, recStack)) {
                    return true; 
                }
            }

            else if (recStack[neighborId]) {
                return true; 
            }
        }

        recStack[id] = false;  
        return false;
    }

    
  public:

    const vector<VertexType>& getVertices() const{
        return vertexVec;
    }

    const vector<list<pair<VertexType *, int>>>& getAdjList() const{
        return adjList;
    }

    const vector<std::vector<double>>& getAdjMatrix() const{
        return adjMatrix;
    }

    bool isDirectGraph() const{
        return isDirect;
    }

    GraphRepresentationType getRepresentation() const{
        return rep;
    }

    int getVerticesNum() const{
        return verticesNum;
    }



    Graph(bool isDirect, bool hasEdgeWeight, GraphRepresentationType rep, vector<EdgeType> &edges, vector<VertexType> &vertexVec) : verticesNum(vertexVec.size()), hasEdgeWeight(hasEdgeWeight), isDirect(isDirect), rep(rep), vertexVec(vertexVec) {

        if (rep == GraphRepresentationType::MATRIX_FORM) {
            initAdjMatrix(edges, verticesNum);
            hasAdjMatrix = true;
        } else if (rep == GraphRepresentationType::LIST_FORM) {
            initAdjList(edges, verticesNum);
            hasAdjList = true;
        }
    }

    void initId() {
        int idCount = 0;
        for (VertexType &vertex : vertexVec) {
            vertex.setId(idCount);
            idCount++;
        }
    }

    void initAdjList(vector<EdgeType> &edges, int verticesNum) {
        adjList.resize(verticesNum);
        for (const EdgeType &edge : edges) {
            VertexType u = *edge.getFirst();
            VertexType v = *edge.getSecond();
            double weight = edge.getWeight();
            list<pair<VertexType *, double>> &target_list = adjList[u.getId()];
            target_list.push_back({edge.getSecond(), weight});
        }
    }

    void initAdjMatrix(vector<EdgeType> &edges, int verticesNum) {
        adjMatrix.resize(verticesNum, std::vector<double>(verticesNum, 0));
        for (const EdgeType &edge : edges) {
            VertexType u = *edge.getFirst();
            VertexType v = *edge.getSecond();
            double weight = edge.getWeight();
            if (!hasEdgeWeight) {
                weight = 1;
            }
            adjMatrix[u.getId()][v.getId()] = weight;
        }
    }

    std::vector<int> dfs(VertexType* start){
        if (!hasAdjList) {
            throw std::runtime_error("Adjacency list representation is required for DFS.");
        }

        std::vector<bool> visited(verticesNum+1, false); 
        std::vector<int> result;                     


        dfsHelper(start, visited, result);


        return result;
    }

    int longestPath(VertexType* start){
        if (!hasAdjList) {
            throw std::runtime_error("Adjacency list representation is required for longest path computation.");
        }

        vector<int> topOrder=dfs(start);
        std::reverse(topOrder.begin(),topOrder.end());

        vector<int> distance(verticesNum,INT_MIN);
        distance[start->getId()]=start->getWeight();

        for(int id:topOrder){
            if(distance[id]!=INT_MIN){
                for (const auto& [neighbor, weight] : adjList[id]) {
                    int vId = neighbor->getId();
                    int vertexWeight=neighbor->getWeight();
                    distance[vId] = std::max(distance[vId], distance[id] + vertexWeight);
                }
            }
        }
        int longestPath = INT_MIN;
        for (int d : distance) {
            if (d != INT_MIN) {
                longestPath = std::max(longestPath, d);
            }
        }

        return longestPath;
    }

    int longestPath(){
        vector<int> longestPaths;
        for(VertexType&vertex:vertexVec){
            int longest_path=longestPath(&vertex);
            longestPaths.push_back(longest_path);
        }
        int longestPath = INT_MIN;
        for (int d : longestPaths) {
            if (d != INT_MIN) {
                longestPath = std::max(longestPath, d);
            }
        }

        return longestPath;
    }

    bool hasCircle(){
        if(vertexVec.size()<1){
            return false;
        }

        vector<bool> visited(verticesNum, false); 
        vector<bool> recStack(verticesNum, false); 

        for (auto& vertex : vertexVec) {
            if (!visited[vertex.getId()]) {
                if (dfsUtilForCycle(&vertex, visited, recStack)) {
                    return true; 
                }
            }
        }

        return false; 
    }

    void addVertex(VertexType &vertex) {
        vertex.setId(verticesNum++);
        vertexVec.push_back(vertex);
        adjList.resize(verticesNum);
        if (hasAdjMatrix) {
            for (auto &row : adjMatrix) {
                row.resize(verticesNum, 0);
            }
            adjMatrix.push_back(vector<double>(verticesNum, 0));
        }

        if (hasAdjList) {
            adjMatrix.emplace_back(verticesNum, 0);
        }
    }

    void deleteVertex(int vertexId) {
        if (vertexId < 0 || vertexId >= verticesNum) return;

        
        if (hasAdjList) {
            
            adjList[vertexId].clear();
            for (auto &neighbors : adjList) {
                neighbors.remove_if([vertexId](const auto &pair) { return pair.first->getId() == vertexId; });
            }

           
            adjList.erase(adjList.begin() + vertexId);

           
            for (int i = vertexId; i < adjList.size(); ++i) {
                for (auto &pair : adjList[i]) {
                    if (pair.first->getId() > vertexId) {
                        pair.first->setId(pair.first->getId() - 1);
                    }
                }
            }
        }

        if (hasAdjMatrix) {
           
            adjMatrix.erase(adjMatrix.begin() + vertexId);
            for (auto &row : adjMatrix) {
                row.erase(row.begin() + vertexId);
            }
        }

        
        vertexVec.erase(vertexVec.begin() + vertexId);
        --verticesNum;

        
        for (int i = vertexId; i < verticesNum; ++i) {
            vertexVec[i].setId(i);
        }
    }

    void addEdge(VertexType *u, VertexType *v, double weight) {
        if (hasAdjList) {
            adjList[u->getId()].emplace_back(v, weight);
            if (!isDirect) {
                adjList[v->getId()].emplace_back(u, weight);
            }
        } else if (hasAdjMatrix) {
            adjMatrix[u->getId()][v->getId()] = weight;
            if (!hasEdgeWeight) {
                weight = 1;
            }
            if (!isDirect) {
                adjMatrix[v->getId()][u->getId()] = weight;
            }
        }
    }

    void deleteEdge(VertexType *u, VertexType *v) {
        if (hasAdjList) {
            adjList[u->getId()].remove_if([v](const auto &pair) { return pair.first == v; });
            if (!isDirect) {
                adjList[v->getId()].remove_if([u](const auto &pair) { return pair.first == u; });
            }
        } else if (hasAdjMatrix) {
            adjMatrix[u->getId()][v->getId()] = 0;
            if (!isDirect) {
                adjMatrix[v->getId()][u->getId()] = 0;
            }
        }
    }

    void printAdjList() const {
        for (int i = 0; i < verticesNum; i++) {
            const std::list<pair<VertexType *, double>> &targetList = adjList[i];

            const VertexType &targetVertex = vertexVec[i];
            cout << targetVertex;
            for (const pair<VertexType *, double> &target_pair : targetList) {
                cout << " -> " << *target_pair.first;
                if (hasEdgeWeight) {
                    cout << ", edge weight:" << target_pair.second;
                }
            }
            cout << "\n";
        }
    }

    void printAdjMatrix() const {
        for (int i = 0; i < verticesNum; i++) {
            for (int j = 0; j < verticesNum; j++) {
                double num = adjMatrix[i][j];
                cout << num << " ";
            }
            cout << "\n";
        }
    }

    static vector<Vertex<>> convertFromWeight(vector<double>& weight){
        vector<Vertex<>> result;
        for(int i=0;i<weight.size();i++){
            result.push_back(Vertex<>(i,weight[i]));
        }
        return result;
    }

    static vector<Edge<>> convertFromMatrix(vector<Vertex<>>& vertex,vector<vector<int>>& edges){
        vector<Edge<>> result;
        for(int i=0;i<edges.size();i++){
            for(int j=0;j<edges[i].size();j++){
                if(edges[i][j]>0){
                    result.push_back(Edge<>(vertex[i],vertex[j],edges[i][j]));
                }
            }
        }
        return result;
    }
};

#endif