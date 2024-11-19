#ifndef IOMANAGer_HPP
#define IOMANAGer_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

#include "Graph.hpp"

template <typename DataType = std::monostate>
class IOManager {
  public:
    using VertexType = Vertex<DataType>;
    using EdgeType = Edge<DataType>;

    static void loadGraphFromFile(
        const std::string& filename,
        std::vector<VertexType>& vertices,
        std::vector<EdgeType>& edges) {
        std::ifstream infile(filename);

        if (!infile.is_open()) {
            throw std::runtime_error("无法打开文件：" + filename);
        }

        std::string line;
        std::unordered_map<std::string, VertexType*> vertexMap; // 用于存储顶点的映射

        while (std::getline(infile, line)) {
            // 跳过空行或以 '#' 开头的注释行
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            std::string node1, node2;
            double weight;

            if (!(iss >> node1 >> node2 >> weight)) {
                throw std::runtime_error("Failed to parse lines" + line);
            }

            // 检查或创建第一个顶点
            if (vertexMap.find(node1) == vertexMap.end()) {
                VertexType* v1 = new VertexType(static_cast<int>(vertexMap.size()));
                v1->setId(vertexMap.size());
                vertexMap[node1] = v1;
                vertices.push_back(*v1);
            }

            // 检查或创建第二个顶点
            if (vertexMap.find(node2) == vertexMap.end()) {
                VertexType* v2 = new VertexType(static_cast<int>(vertexMap.size()));
                v2->setId(vertexMap.size());
                vertexMap[node2] = v2;
                vertices.push_back(*v2);
            }

            // 创建边并添加到 edges 向量中
            VertexType* first = vertexMap[node1];
            VertexType* second = vertexMap[node2];
            edges.emplace_back(*first, *second, weight);
            edges.emplace_back(*second, *first, weight);
        }

        infile.close();
    }
};

#endif
