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


#include <memory>

using std::shared_ptr;
using std::unique_ptr;

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
    Vertex(int id, int weight, const DataType &data) : id(id), weight(weight), hasWeight(true), data(data) {
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
            os << ", Weight: " << vertex.weight;
        }
        os << ")";
        return os;
    }
};

template <typename DataType = std::monostate>
class Edge {
  private:
    bool isDirect = false;
    std::shared_ptr<Vertex<DataType>> first;
    std::shared_ptr<Vertex<DataType>> second;
    std::shared_ptr<Edge> residualEdge;
    double weight;
    double capacity=0;
    double flow=0;

  public:
    Edge(std::shared_ptr<Vertex<DataType>> first, std::shared_ptr<Vertex<DataType>> second, double weight, bool isDirect) : first(std::move(first)), second(std::move(second)), weight(weight), isDirect(isDirect) {
    }

    Edge(std::shared_ptr<Vertex<DataType>> first, std::shared_ptr<Vertex<DataType>> second, double weight) : first(std::move(first)), second(std::move(second)), weight(weight), isDirect(false) {
    }

    Edge(std::shared_ptr<Vertex<DataType>> first, std::shared_ptr<Vertex<DataType>> second, bool isDirect) : first(std::move(first)), second(std::move(second)), weight(1), isDirect(isDirect) {
    }

    Edge(std::shared_ptr<Vertex<DataType>> first, std::shared_ptr<Vertex<DataType>> second) : first(std::move(first)), second(std::move(second)), weight(1), isDirect(false) {
    }

    std::shared_ptr<Edge<DataType>> getResidualEdge(){
        return residualEdge;
    }

    void setResidualEdge(std::shared_ptr<Edge<DataType>> edge){
        residualEdge=edge;
    }

    std::shared_ptr<Vertex<DataType>> getFirst() const {
        return first;
    }
    std::shared_ptr<Vertex<DataType>> getSecond() const {
        return second;
    }
    double getWeight() const {
        return weight;
    }
    bool getIsDirect() const {
        return isDirect;
    }

    double getCapacity() const {
        return capacity;
    }

    double getFLow() const {
        return flow;
    }

    double getResidualCapacity() const{
        return (capacity-flow);
    }

    void addFlow(double value) {
        flow += value;
        if (flow > capacity) {
            throw std::overflow_error("Invalid flow");
        }
    }

    void setCapacity(double capacity) {
        this->capacity=capacity;
    }

    void setWeight(double newWeight) {
        weight = newWeight;
    }
    void setIsDirect(bool direction) {
        isDirect = direction;
    }

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge) {
        os << "Edge(" << edge.first->getId() << " -> " << edge.second->getId() << ", Weight: " << edge.weight << ", Directed: " << (edge.isDirect ? "Yes" : "No") << ")";
        return os;
    }
};

template <typename DataType = std::monostate>
class Graph {
  private:
    using EdgeType = Edge<DataType>;
    using VertexType = Vertex<DataType>;

    int verticesNum; // Number of nodes
    bool isDirect;   // Directed or not
    GraphRepresentationType rep;

    std::vector<std::shared_ptr<VertexType>> vertexVec;
    std::vector<std::shared_ptr<EdgeType>> edgeVec;
    std::vector<std::list<std::shared_ptr<EdgeType>>> adjList; // Adjacency List
    std::vector<std::vector<double>> adjMatrix;                // Adjacency Matrix

    bool hasAdjList = false;
    bool hasAdjMatrix = false;
    bool hasEdgeWeight = false;

    void dfsHelper(const std::shared_ptr<VertexType> &v, std::vector<bool> &visited, std::vector<int> &result) {
        visited[v->getId()] = true;
        for (const auto &edge : adjList[v->getId()]) {
            auto neighbor = edge->getSecond();
            if (!visited[neighbor->getId()]) {
                dfsHelper(neighbor, visited, result);
            }
        }
        result.push_back(v->getId());
    }

  public:
    Graph(bool isDirect, bool hasEdgeWeight, GraphRepresentationType rep, std::vector<std::shared_ptr<EdgeType>> edges, std::vector<std::shared_ptr<VertexType>> vertices) : verticesNum(vertices.size()), isDirect(isDirect), rep(rep), hasEdgeWeight(hasEdgeWeight), vertexVec(std::move(vertices)), edgeVec(std::move(edges)) {
        if (rep == GraphRepresentationType::MATRIX_FORM) {
            initAdjMatrix(edgeVec, verticesNum);
            hasAdjMatrix = true;
        } else if (rep == GraphRepresentationType::LIST_FORM) {
            initAdjList(edgeVec, verticesNum);
            hasAdjList = true;
            initAdjMatrix(edgeVec, verticesNum);
            hasAdjMatrix = true;
        }
    }

    void initAdjList(const std::vector<std::shared_ptr<EdgeType>> &edges, int verticesNum) {
        adjList.resize(verticesNum);
        for (const auto &edge : edges) {
            adjList[edge->getFirst()->getId()].push_back(edge);
            if (!isDirect) {
                adjList[edge->getSecond()->getId()].push_back(std::make_shared<EdgeType>(edge->getSecond(), edge->getFirst(), edge->getWeight()));
            }
        }
    }

    void initAdjMatrix(const std::vector<std::shared_ptr<EdgeType>> &edges, int verticesNum) {
        adjMatrix.resize(verticesNum, std::vector<double>(verticesNum, 0));
        for (const auto &edge : edges) {
            adjMatrix[edge->getFirst()->getId()][edge->getSecond()->getId()] = edge->getWeight();
            if (!isDirect) {
                adjMatrix[edge->getSecond()->getId()][edge->getFirst()->getId()] = edge->getWeight();
            }
        }
    }

    void addVertex(const std::shared_ptr<VertexType> &vertex) {
        vertex->setId(verticesNum++);
        vertexVec.push_back(vertex);
        adjList.resize(verticesNum);
        if (hasAdjMatrix) {
            for (auto &row : adjMatrix) {
                row.resize(verticesNum, 0);
            }
            adjMatrix.push_back(std::vector<double>(verticesNum, 0));
        }
    }

    void addEdge(const std::shared_ptr<VertexType> &u, const std::shared_ptr<VertexType> &v, double weight) {
        auto newEdge = std::make_shared<EdgeType>(u, v, weight, isDirect);
        edgeVec.push_back(newEdge);

        if (hasAdjList) {
            adjList[u->getId()].push_back(newEdge);
            if (!isDirect) {
                adjList[v->getId()].push_back(std::make_shared<EdgeType>(v, u, weight));
            }
        }
        if (hasAdjMatrix) {
            adjMatrix[u->getId()][v->getId()] = weight;
            if (!isDirect) {
                adjMatrix[v->getId()][u->getId()] = weight;
            }
        }
    }

    std::vector<int> dfs(const std::shared_ptr<VertexType> &start) {
        if (!hasAdjList) {
            throw std::runtime_error("Adjacency list representation is required for DFS.");
        }

        std::vector<bool> visited(verticesNum, false);
        std::vector<int> result;

        dfsHelper(start, visited, result);

        return result;
    }

    void printAdjList() const {
        for (size_t i = 0; i < adjList.size(); ++i) {
            std::cout << *vertexVec[i];
            for (const auto &edge : adjList[i]) {
                std::cout << " -> " << *edge->getSecond();
                if (hasEdgeWeight) {
                    std::cout << " (weight: " << edge->getWeight() << ")";
                }
            }
            std::cout << "\n";
        }
    }

    void printAdjMatrix() const {
        for (const auto &row : adjMatrix) {
            for (const auto &val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }

    const std::vector<std::shared_ptr<Vertex<DataType>>>& getVertices() const {
        return vertexVec;
    }

    const std::vector<std::shared_ptr<Edge<DataType>>>& getEdges() const {
        return edgeVec;
    }

    const std::vector<std::list<std::shared_ptr<Edge<DataType>>>>& getAdjList() const {
        if (!hasAdjList) {
            throw std::runtime_error("Adjacency list representation is not initialized.");
        }
        return adjList;
    }


    const std::vector<std::vector<double>>& getAdjMatrix() const {
        if (!hasAdjMatrix) {
            throw std::runtime_error("Adjacency matrix representation is not initialized.");
        }
        return adjMatrix;
    }

    const std::shared_ptr<Vertex<DataType>>& getVertex(int id) const {
        if (id < 0 || id >= verticesNum) {
            throw std::out_of_range("Vertex ID is out of range.");
        }
        return vertexVec[id];
    }


    const std::shared_ptr<Edge<DataType>>& getEdge(int fromId, int toId) const {
        if (!hasAdjList) {
            throw std::runtime_error("Adjacency list representation is required to get an edge.");
        }
        for (const auto &edge : adjList[fromId]) {
            if (edge->getSecond()->getId() == toId) {
                return edge;
            }
        }
        throw std::runtime_error("Edge not found.");
    }
};

#endif