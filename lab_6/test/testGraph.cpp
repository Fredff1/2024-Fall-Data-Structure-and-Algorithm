#include "../include/Graph.h"

void test() {
    Vertex<> v0(0), v1(1), v2(2), v3(3);

    // 创建边
    Edge<> e1(v0, v1, 5);
    Edge<> e2(v0, v2, 3);
    Edge<> e3(v1, v3, 8);
    Edge<> e4(v2, v3, 2);

    // 边和顶点的向量
    std::vector<Edge<>> edges = {e1, e2, e3, e4};
    std::vector<Vertex<>> vertices = {v0, v1, v2, v3};

    // 使用邻接表表示构造有向图
    Graph<> graphList(true, true, GraphRepresentationType::LIST_FORM, edges, vertices);
    std::cout << "Initial graph (Adjacency List):\n";
    graphList.printAdjList();
    graphList.addEdge(&v0, &v3, 7); // 测试添加边
    graphList.deleteEdge(&v0, &v1); // 测试删除边
    auto newVertex_1 = Vertex<>(4);
    graphList.addVertex(newVertex_1); // 测试添加顶点
    graphList.deleteVertex(2);        // 测试删除顶点

    // 使用邻接矩阵表示构造无向图

    Graph<> graphMatrix(false, false, GraphRepresentationType::MATRIX_FORM, edges, vertices);
    std::cout << "\nInitial graph (Adjacency Matrix):\n";
    graphMatrix.printAdjMatrix();
    graphMatrix.addEdge(&v0, &v3, 7); // 测试添加边
    graphMatrix.deleteEdge(&v0, &v1); // 测试删除边
    auto newVertex_2 = Vertex<>(4);
    graphMatrix.addVertex(newVertex_2); // 测试添加顶点
    graphMatrix.deleteVertex(2);        // 测试删除顶点

    // 输出完成的图结构
    std::cout << "\nAdjacency List Representation:\n";
    graphList.printAdjList();

    std::cout << "\nAdjacency Matrix Representation:\n";
    graphMatrix.printAdjMatrix();
}

int main() {
    test();
    return 0;
}