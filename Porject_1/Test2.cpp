#include "TreeMap.h"
#include <cassert>

int main() {
    // 基础功能测试
    TreeMap<int, std::string> myMap;
    auto *val_2=myMap.insert(1, "one");
    myMap.insert(2, "two");
    myMap.insert(3, "three");

    assert(myMap.size()==3);

    std::string *val = myMap.get(1);
    assert(val && *val == "one");
    assert(val_2->getData()=="one");

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
    assert(myMap[2] == "banana");

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
        auto value_=myMap.find(i);
        cout<<value_<<" "<<endl;
        
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
    const int NUM_ELEMENTS = 100000;
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

    return 0;
}