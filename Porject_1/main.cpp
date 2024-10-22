#include "Dict.h"
#include "RBTree.h"
#include "TreeMap.h"
#include <cassert>

double test_insert() {
    auto start = std::chrono::high_resolution_clock::now();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    cout << "Action insert took time " << duration.count() << endl;
    return duration.count();
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    Dictionary dic = Dictionary(RB_TREEMAP_TYPE);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout <<"Action insert took time " << duration.count() <<" micro seconds"<< endl;
    cout << "The number of elements are " << dic.size() << endl;

    auto vec=dic.searchBatch("aaronic","abacus");
    for(auto&str:vec){
        cout<<"key: "<<str.first<<" value: "<<str.second<<"  \n";
    }
    // dic.print();



    auto deleteTarget = FileIO::readFile("2_delete.txt");
    start = std::chrono::high_resolution_clock::now();
    for (const auto &target_pair : deleteTarget.terms) {
        auto targetKey = target_pair.first;
        dic.erase(targetKey);
    }
    end = std::chrono::high_resolution_clock::now();
    cout << "The number of elements are " << dic.size() << endl;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Delete took time " << duration.count()<<" micro seconds" << endl;
    // dic.print(" ");



    auto insertTarget_1 = FileIO::readFile("3_insert.txt");
    start = std::chrono::high_resolution_clock::now();
    for (const auto &target_pair : insertTarget_1.terms) {
        auto targetKey = target_pair.first;
        auto targetValue = target_pair.second;
        dic.insert(targetKey, targetValue);
    }
    end = std::chrono::high_resolution_clock::now();
    duration =std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "insert took time " << duration.count()<<" micro seconds" << endl;
    cout << "The number of elements are " << dic.size()<< endl;
    // dic.print();

    return 0;
}