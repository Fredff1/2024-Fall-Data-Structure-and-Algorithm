#include "Utilility.h"
#include "RBTree.h"


int main() {


    auto treeTest = RedBlackTree<int, int>();

    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(2, 4);
    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(5, 3);
    // treeTest.printTree(treeTest.getRoot());
    treeTest.insert(3, 4);

    for (int k = 0; k < 22000; k++) {
        treeTest.insert(RandomGenerator::getRandomValue<int>(0, 2000), 3);
    }
    vector<int> insert_target = RandomGenerator::getRandomVector<int>(26000, -10000, 10000);
    for (auto j : insert_target) {
        treeTest.insert(j, j);
    }
    for (int i = 0; i < insert_target.size(); i += 1) {
        int rand_num = RandomGenerator::getRandomValue<int>(0, insert_target.size() - 1);
        if (RandomGenerator::getRandomValue<int>(0, 10) < 3) {
            treeTest.insert(RandomGenerator::getRandomValue<int>(-1000, 2000), 3);
        }
        treeTest.deleteNode(insert_target[i]);
    }

    // treeTest.printTree();
    cout << "--------------------------" << endl;
    // treeTest.printTree();

    // treeTest.deleteNode(21);

    bool flag = treeTest.validateRBTree(nullptr);
    if (!flag) {
        // treeTest.printTree();
        cout << "delete " << " caused exception" << endl;
        cout << endl;
        return 1;
    }
    cout << "--------------------------" << endl;

    // 有序插入测试
    for (int i = 0; i < 10000; ++i) {
        treeTest.insert(i, i);
    }
    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Ordered insert test failed!" << endl;
        return 1;
    }

    // 逆序插入测试
    for (int i = 10000; i > 0; --i) {
        treeTest.insert(i, i);
    }
    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Reverse ordered insert test failed!" << endl;
        return 1;
    }

    treeTest.insert(INT_MIN, 1); // 插入最小整数
    treeTest.insert(INT_MAX, 1); // 插入最大整数
    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Boundary value test failed!" << endl;
        return 1;
    }

    for (int i = 0; i < 5000; ++i) {
        treeTest.insert(i, i);
    }
    for (int i = 0; i < 5000; ++i) {
        treeTest.deleteNode(i); // 删除插入的每个元素
    }
    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Mass deletion test failed!" << endl;
        return 1;
    }
    for (int i = 0; i < 1000; ++i) {
        treeTest.insert(10, i); // 重复插入相同的键
    }
    treeTest.deleteNode(10); // 删除这个键
    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Repeated key insert/delete test failed!" << endl;
        return 1;
    }
    treeTest.insert(INT_MIN, 0); // 插入极小值
    treeTest.insert(INT_MAX, 0); // 插入极大值

    // 删除极端值
    treeTest.deleteNode(INT_MIN);
    treeTest.deleteNode(INT_MAX);

    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Extreme value test failed!" << endl;
        return 1;
    }

    for (int i = 0; i < 10000; ++i) {
        treeTest.insert(i, i);
        if (i % 2 == 0) {
            treeTest.deleteNode(i / 2); // 每插入两个元素，删除一个
        }
    }

    if (!treeTest.validateRBTree(nullptr)) {
        cout << "Alternating insert/delete test failed!" << endl;
        return 1;
    }

    return 0;
}