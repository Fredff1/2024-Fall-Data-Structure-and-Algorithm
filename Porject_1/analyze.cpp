#include "src/Dict.h"
#include "src/RBTree.h"
#include "src/RBTreeMap.h"
#include <cassert>


std::string split_str=std::string(100,'-');

double test_insert() {
    auto start = std::chrono::high_resolution_clock::now();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Action insert took time " << duration.count()<<"micro seconds" << endl;
    return duration.count();
}

void test(MapType type){
    
    cout<<split_str<<endl;
    cout<<"Analysis begins"<<endl;
    cout<<split_str<<endl;

    auto insertTarget_0 = FileIO::readFile("./1_initial.txt");
    Dictionary dic = Dictionary(type);

    cout << "Inserting elements from 1_initial.txt:" << endl;
    int count = 0;
    auto totalStart = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto &target_pair : insertTarget_0.terms) {
        auto targetKey = target_pair.first;
        auto targetValue = target_pair.second;
        dic.insert(targetKey, targetValue);

        count++;
        if (count % 100 == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            cout << "Inserted " << 100 << " elements, took time " << duration.count() << " microseconds" << endl;
            start = std::chrono::high_resolution_clock::now(); // Reset timer
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - totalStart);
    //cout <<"Total insert from 1_initial.txt took time " << duration.count() <<" microseconds"<< endl;
    cout << "The number of elements are " << dic.size() << endl;

    // Deletion process
    cout << "Deleting elements from 2_delete.txt:" << endl;
    count = 0;
    auto deleteTarget = FileIO::readFile("./2_delete.txt");
    start = std::chrono::high_resolution_clock::now();
    totalStart = std::chrono::high_resolution_clock::now();
    for (const auto &target_pair : deleteTarget.terms) {
        auto targetKey = target_pair.first;
        dic.erase(targetKey);

        count++;
        if (count % 100 == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - totalStart);
            cout << "Deleted " << 100 << " elements, took time " << duration.count() << " microseconds" << endl;
            start = std::chrono::high_resolution_clock::now(); // Reset timer
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //cout << "Total delete by 2_delete.txt took time " << duration.count()<<" microseconds" << endl;

    // Insert from 3_insert.txt
    cout << "Inserting elements from 3_insert.txt:" << endl;
    count = 0;
    auto insertTarget_1 = FileIO::readFile("./3_insert.txt");
    start = std::chrono::high_resolution_clock::now();
    totalStart = std::chrono::high_resolution_clock::now();
    for (const auto &target_pair : insertTarget_1.terms) {
        auto targetKey = target_pair.first;
        auto targetValue = target_pair.second;
        dic.insert(targetKey, targetValue);

        count++;
        if (count % 100 == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - totalStart);
            cout << "Inserted " << 100 << " elements, took time " << duration.count() << " microseconds" << endl;
            start = std::chrono::high_resolution_clock::now(); // Reset timer
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration =std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //cout << "Total insert by 3_insert.txt took time " << duration.count()<<" microseconds" << endl;

    cout<<split_str<<endl;
    cout<<"Analysis end"<<endl;
    cout<<split_str<<endl;
    
}

int main() {
    cout<<split_str<<endl;
    cout<<"Now testing RBTree\n";
    try{
        test(RB_TREEMAP_TYPE);
    }catch(std::exception& e){
        cout<<e.what();
    }
    cout<<split_str<<endl;
    cout<<"Now testing Btree\n";
    try{
        test(B_TREEMAP_TYPE);
    }catch(std::exception& e){
        cout<<e.what();
    }
    cout<<split_str<<endl;
    

    return 0;
}