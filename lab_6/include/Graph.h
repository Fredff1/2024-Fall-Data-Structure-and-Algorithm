#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <optional>
#include <unordered_map>
#include <tuple>
#include <variant>
#include "Utilility.h"

using std::list;
using std::tuple;
using std::pair;

enum class GraphRepresentationType{
    LIST_FORM, 
    MATRIX_FORM
};

template<typename DataType=std::monostate>
class Vertex{
  private:
    int id;
    int weight;  // 顶点的权重
    std::optional<DataType> data;  // 顶点的附加数据，使用std::optional来支持可选性

  public:

    Vertex() : id(-1), weight(0) {}

    Vertex(int id) : id(id), weight(0) {}

    Vertex(int id, int weight) : id(id), weight(weight) {}

    Vertex(int id, int weight, DataType data) : id(id), weight(weight), data(data) {}

    int getId() const {
        return id;
    }

    void setId(int id){
        this->id=id;
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

    void setData(const DataType& newData) {
        data = newData;
    }


    friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
        os << "Vertex(ID: " << vertex.id << ", vertex weight: " << vertex.weight;
       
        os << ")";
        return os;
    }

    template<typename U>
    Vertex(const Vertex<U>& other) : id(other.getId()), weight(other.getWeight()) {}
};

template<typename DataType=std::monostate>
class Edge{
  private:
    bool isDirect=false;
    Vertex<DataType>* first;
    Vertex<DataType>* second;
    int weight;

  public:
     // 完整构造函数，包含顶点、权重和方向
    Edge(Vertex< DataType>& first, Vertex<DataType>& second, int weight, bool isDirect)
        : first(&first), second(&second), weight(weight), isDirect(isDirect) {}

    // 构造函数，包含顶点和权重，默认为无向边
    Edge(Vertex<DataType>& first, Vertex< DataType>& second, int weight)
        : first(&first), second(&second), weight(weight), isDirect(false) {}

    // 构造函数，包含顶点和方向，权重默认为1
    Edge(Vertex<DataType>& first, Vertex< DataType>& second, bool isDirect)
        : first(&first), second(&second), weight(1), isDirect(isDirect) {}

    // 构造函数，仅包含顶点，默认权重为1，默认为无向边
    Edge(Vertex< DataType>& first, Vertex<DataType>& second)
        : first(&first), second(&second), weight(1), isDirect(false) {}

    // 获取起点
    Vertex<DataType>* getFirst() const { return first; }

    // 获取终点
    Vertex<DataType>* getSecond() const { return second; }

    // 获取边的权重
    int getWeight() const { return weight; }

    // 获取是否为有向边
    bool getIsDirect() const { return isDirect; }

    // 设置权重
    void setWeight(int newWeight) { weight = newWeight; }

    // 设置方向
    void setIsDirect(bool direction) { isDirect = direction; }

    // 重载输出运算符，便于调试
    friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        os << "Edge(" << edge.first.id << " -> " << edge.second.id 
           << ", Weight: " << edge.weight 
           << ", Directed: " << (edge.isDirect ? "Yes" : "No") << ")";
        return os;
    }
    
};




template<typename DataType=std::monostate>
class Graph{
    using EdgeType=Edge<DataType>;
    using VertexType=Vertex<DataType>;
  private:
    int verticesNum; //num of nodes
    bool isDirect; //has direction
    GraphRepresentationType rep;
    
    vector<VertexType> vertexVec;
    vector<list<pair<VertexType*,int>>> adjList; // Adjacency List
    vector<std::vector<int>> adjMatrix;  //Adjacency Matrix



    bool hasAdjList=false;
    bool hasAdjMatrix=false;


  public:
    Graph(bool isDirect,GraphRepresentationType rep,vector<EdgeType>& edges,vector<VertexType>& vertexVec):
    verticesNum(vertexVec.size()),isDirect(isDirect),rep(rep),vertexVec(vertexVec){
        
        if(rep==GraphRepresentationType::MATRIX_FORM){
            initAdjMatrix(edges,verticesNum);
            hasAdjMatrix=true;
        }else if(rep==GraphRepresentationType::LIST_FORM){
            initAdjList(edges,verticesNum);
            hasAdjList=true;
        }
    }

    void initId(){
        int idCount=0;
        for(VertexType& vertex:vertexVec){
            vertex.setId(idCount);
            idCount++;
        }
    }

    


    void initAdjList(vector<EdgeType>& edges,int verticesNum){
        adjList.resize(verticesNum);
        for(const EdgeType&edge:edges){
            VertexType u=*edge.getFirst();
            VertexType v=*edge.getSecond();
            int weight=edge.getWeight();
            list<pair<VertexType*,int>>& target_list=adjList[u.getId()];
            target_list.push_back({edge.getSecond(),weight});
        }
        
       
    }

    void initAdjMatrix(vector<EdgeType>& edges,int verticesNum){
        adjMatrix.resize(verticesNum, std::vector<int>(verticesNum, 0));
        for(const EdgeType&edge:edges){
            VertexType u=*edge.getFirst();
            VertexType v=*edge.getSecond();
            int weight=edge.getWeight();
            adjMatrix[u.getId()][v.getId()]=weight;
        }
    }

    void addVertex(VertexType& vertex){
        vertex.setId(verticesNum++);
        vertexVec.push_back(vertex);
        adjList.resize(verticesNum);
        if (hasAdjMatrix){
            for (auto& row : adjMatrix) {
                row.resize(verticesNum, 0);
            }
            adjMatrix.push_back(vector<int>(verticesNum,0));
        }

        if(hasAdjList){
            adjMatrix.emplace_back(verticesNum, 0);
        }
        
        
    }

    void deleteVertex(int vertexId) {
        if (vertexId < 0 || vertexId >= verticesNum) return;

        // 删除邻接表中的边
        if (hasAdjList) {
            // 移除与该顶点相关的所有边
            adjList[vertexId].clear();
            for (auto& neighbors : adjList) {
                neighbors.remove_if([vertexId](const auto& pair) { return pair.first->getId() == vertexId; });
            }

            // 从 `adjList` 中删除该顶点的条目
            adjList.erase(adjList.begin() + vertexId);

            // 更新所有后续顶点的 ID 并修正指向
            for (int i = vertexId; i < adjList.size(); ++i) {
                for (auto& pair : adjList[i]) {
                    if (pair.first->getId() > vertexId) {
                        pair.first->setId(pair.first->getId() - 1);
                    }
                }
            }
        }

        // 删除邻接矩阵中的边
        if (hasAdjMatrix) {
            // 从矩阵中删除该行和该列
            adjMatrix.erase(adjMatrix.begin() + vertexId);
            for (auto& row : adjMatrix) {
                row.erase(row.begin() + vertexId);
            }
        }

        // 从顶点列表中删除该顶点并更新顶点计数
        vertexVec.erase(vertexVec.begin() + vertexId);
        --verticesNum;

        // 更新剩余顶点的 ID
        for (int i = vertexId; i < verticesNum; ++i) {
            vertexVec[i].setId(i);
        }
    }

    void addEdge(VertexType* u, VertexType* v, int weight) {
        if (hasAdjList) {
            adjList[u->getId()].emplace_back(v, weight);
            if (!isDirect) {
                adjList[v->getId()].emplace_back(u, weight);
            }
        } else if (hasAdjMatrix) {
            adjMatrix[u->getId()][v->getId()] = weight;
            if (!isDirect) {
                adjMatrix[v->getId()][u->getId()] = weight;
            }
        }
    }

    void deleteEdge(VertexType* u, VertexType* v) {
        if (hasAdjList) {
            adjList[u->getId()].remove_if([v](const auto& pair) { return pair.first == v; });
            if (!isDirect) {
                adjList[v->getId()].remove_if([u](const auto& pair) { return pair.first == u; });
            }
        } else if (hasAdjMatrix) {
            adjMatrix[u->getId()][v->getId()] = 0;
            if (!isDirect) {
                adjMatrix[v->getId()][u->getId()] = 0;
            }
        }
    }

    void printAdjList() const{
        for(int i=0;i<verticesNum;i++){
            const std::list<pair<VertexType*,int>>& targetList=adjList[i];
            
            const VertexType& targetVertex=vertexVec[i];
            cout<<targetVertex;
            for(const pair<VertexType*,int>& target_pair:targetList){
                cout<<" -> "<<*target_pair.first<<", edge weight:"<<target_pair.second;
            }
            cout<<"\n";
        }
    }

    void printAdjMatrix() const{
        for(int i=0;i<verticesNum;i++){
            for(int j=0;j<verticesNum;j++){
                int num=adjMatrix[i][j];
                cout<<num<<" ";
            }
            cout<<"\n";
        }
    }
};


