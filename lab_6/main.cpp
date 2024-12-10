#include "FLow.hpp"

int main(){
    string path="./1.txt";
    GraphFlow flow(path);
    auto result=flow.minCostMaxFlow();
    cout<<"Reading from "<<path<<endl;
    cout<<result.maxFlow<<" "<<result.minCost<<endl;

}