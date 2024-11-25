#ifndef IOMANAGer_HPP
#define IOMANAGer_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "Graph.hpp"

using std::unique_ptr,std::shared_ptr;

template <typename DataType = std::monostate>
class IOManager {
  public:
    using VertexType = Vertex<DataType>;
    using EdgeType = Edge<DataType>;

    
};

#endif
