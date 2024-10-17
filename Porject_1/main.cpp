#include "RBTree.h"
#include "TreeMap.h"
#include <cassert>
int main() {

    auto treeTest = RedBlackTree<int, int>();

    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(2, 4);
    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(5, 3);
    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(3, 4);
    treeTest.insert(5, 4);
    treeTest.insert(3, 4);
    treeTest.insert(36, 4);
    treeTest.insert(6, 4);
    treeTest.insert(2, 4);
    treeTest.insert(32, 4);
    treeTest.insert(12, 4);
    auto* node_12=treeTest.findByIndex(7); // should be 12
    treeTest.deleteNode(3);
    auto* node_12_1=treeTest.findByIndex(6); // should be 12
    
    treeTest.printTree();

    RedBlackTree treeTest_2=RedBlackTree<int,std::string>();
    for(int i=0;i<2000;i+=3){
        treeTest_2.insert(i,"hello");
    }

    auto* node_1=treeTest_2.findByIndex(499);

    for(int j=0;j<150;j+=3){
        treeTest_2.deleteNode(j);
    }

    auto* node_2=treeTest_2.findByIndex(449);
    if(node_1==node_2){
        cout<<"same!"<<endl;
    }

    treeTest_2.validateRBTree();

    TreeMap<int, std::string> myMap;

myMap.insert(1, "one");
myMap.insert(2, "two");
myMap.insert(3, "three");

std::string* val = myMap.find(1);
assert(val && *val == "one");  // 查找 key 1 的值，应该返回 "one"

val = myMap.find(2);
assert(val && *val == "two");  // 查找 key 2 的值，应该返回 "two"

val = myMap.find(3);
assert(val && *val == "three");  // 查找 key 3 的值，应该返回 "three"

val = myMap.find(4);
assert(val == nullptr);  // 查找 key 4，不存在，应该返回 nullptr


myMap[1] = "apple";
assert(myMap[1] == "apple");  // 通过 operator[] 插入并访问 key 1

myMap[2] = "banana";
assert(myMap[2] == "banana");  // 通过 operator[] 插入并访问 key 2

std::string defaultValue = myMap[3];  // 自动插入 key 3，值为默认构造的空字符串
assert(defaultValue == "");

    
    return 0;
}