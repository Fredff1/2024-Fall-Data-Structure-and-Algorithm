#include "../src/BtreeMap.h"

#include "../src/Utilility.h"
#include <cassert>


void testAll(){
    BTreeMap<int, std::string> myMap;
    myMap.insert(2, "two");
    myMap.insert(3, "three");

    assert(myMap.size()==2);

    std::string *val = myMap.get(2);
    assert(val && *val == "two");

    val = myMap.get(2);
    assert(val && *val == "two");

    val = myMap.get(3);
    assert(val && *val == "three");

    val = myMap.get(4);
    assert(val == nullptr);

    // operator[] 测试
    myMap[1] = "apple";
    assert(myMap[1] == "apple");

    myMap[2] = "banana";
    cout<<myMap[2]<<endl;
    assert(myMap[2] == "banana");

    myMap[2] = "strawberry";
    assert(myMap[2]=="strawberry");
    cout<<myMap[2]<<endl;

    std::string defaultValue = myMap[3];
    assert(defaultValue == "three");

    // 删除测试
    myMap.erase(2);
    val = myMap.get(2);
    assert(val == nullptr);

    val = myMap.get(1);
    assert(val && *val == "apple");
    assert(myMap.indexOf(1)==0);

    val = myMap.get(3);
    assert(val && *val == "three");

    for(int i=0;i<myMap.size();i++){
        auto value_=myMap.findByIndex(i);
        cout<<value_.first<<" "<<endl;
        
    }

    // 清空测试
    myMap.clear();
    assert(myMap.empty());
    assert(myMap.size() == 0);

    // 覆盖测试
    myMap.insert(1, "first");
    myMap.insert(1, "second");
    val = myMap.get(1);
    assert(val && *val == "second");

    myMap[1] = "first";
    assert(myMap[1] == "first");
    myMap[1] = "second";
    assert(myMap[1] == "second");

    // 大规模测试
    const int NUM_ELEMENTS = 1000;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        myMap.insert(i, std::to_string(i * 2)); // 将 i * 2 转换为字符串插入
    }

    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        std::string *val = myMap.get(i);
        assert(val && *val == std::to_string(i * 2)); // 确认字符串值正确
    }



    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        myMap.erase(i);
        assert(myMap.get(i) == nullptr); // 确认删除成功
    }

    assert(myMap.empty());

    // 边界和异常测试
    try {
        myMap[2]; // 自动插入 key 2，不抛出异常
        std::cout << "Test passed for key 2 automatic insertion.\n";
    } catch (...) {
        std::cout << "Test failed!";
    }

    std::cout << "All tests passed!" << std::endl;

}

void testFunction(){
    BTreeMap<int,int> mymap;
    vector<int> inserttarget;
    for(int i=0;i<1000;i++){
        inserttarget.push_back(i);
    }
    for(int i=0;i<200;i++){
        mymap.insert(i,inserttarget[i]);
    }
    mymap.print();
    auto* value_120=mymap.get(120);
    assert(*value_120==120);
    auto pair_1=mymap.findByIndex(12);
    assert(pair_1.second==12);
    int index_of_12=mymap.indexOf(12);
    assert(index_of_12==12);
    
}

int main() {
    testAll();
    //testFunction();
    
    return 0;
}