#include "include/Graph.h"



void test_1(){
    vector<int> weight={4,5,4,6,4,5,6,7,3,4,2,2,3,2,5};
    vector<Vertex<>> vertex=Graph<>::convertFromWeight(weight);
    vector<vector<int>> matrix=
    {{0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    

    vector<Edge<>> edges=Graph<>::convertFromMatrix(vertex,matrix);

    
    Graph<> graph=Graph<>(true,false,GraphRepresentationType::LIST_FORM,edges,vertex);
    int longest=graph.longestPath();
    cout<<"Longest: "<<longest<<endl;
}

void test_2(){
    vector<int> weight={3,2,2,3,4,3,2,1};
    vector<Vertex<>> vertex=Graph<>::convertFromWeight(weight);
    vector<vector<int>> matrix=
    {{0,0,1,1,0,0,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}};
    

    vector<Edge<>> edges=Graph<>::convertFromMatrix(vertex,matrix);

    
    Graph<> graph=Graph<>(true,false,GraphRepresentationType::LIST_FORM,edges,vertex);
    int longest=graph.longestPath();
    cout<<"Longest: "<<longest<<endl;
   
}

void test_3(){
    vector<int> weight={3,8,9,4,6,10,6,9};
    vector<Vertex<>> vertex=Graph<>::convertFromWeight(weight);
    vector<vector<int>> matrix=
    {{0,0,1,0,1,0,0,0},
    {0,0,0,1,0,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,1,0,1,0,0,0},
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}};
    

    vector<Edge<>> edges=Graph<>::convertFromMatrix(vertex,matrix);

    
    Graph<> graph=Graph<>(true,false,GraphRepresentationType::LIST_FORM,edges,vertex);
    int longest=graph.longestPath();
    cout<<"Longest: "<<longest<<endl;
}

void test_4(){
    vector<int> weight={2,5,5,3,2,1,3,2,6,4,3,4,2};
    vector<Vertex<>> vertex=Graph<>::convertFromWeight(weight);
    vector<vector<int>> matrix=
    {{0,0,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0}};
    

    vector<Edge<>> edges=Graph<>::convertFromMatrix(vertex,matrix);

    
    Graph<> graph=Graph<>(true,false,GraphRepresentationType::LIST_FORM,edges,vertex);
    int longest=graph.longestPath();
    cout<<"Longest: "<<longest<<endl;
}


int main(){
    test_1();
    test_2();
    test_3();
    test_4();
    return 0;
}