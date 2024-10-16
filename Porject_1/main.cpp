#include "RBTree.h"

int main(){
    auto treeTest=RedBlackTree<int,int>();

    //treeTest.printTree(treeTest.getRoot());
    treeTest.insert(2,4);
    //treeTest.printTree(treeTest.getRoot());
    treeTest.insert(5,3);
    //treeTest.printTree(treeTest.getRoot());
    treeTest.insert(3,4);
    treeTest.printTree(treeTest.getRoot());

    treeTest.printTree(treeTest.getRoot());
    vector<int> insert_target={1,3,4,75,34,72,42,15,26,36,367,5,11,43,73,56,28,57,246,334,54,223,434,6543,342,23,64,723,723,6,21,62,114514,73,423,7,23,4,8,1919810,45,23,5,234,5};
    for(auto j:insert_target){
        treeTest.insert(j,j);
    }
    for(int i=0;i<30;i+=1){
        int rand_num=RandomGenerator::getRandomValue<int>(0,insert_target.size()-1);
        cout<<"--------------------------"<<endl;
        treeTest.printTree();
        treeTest.deleteNode(insert_target[rand_num]);
        //treeTest.deleteNode(21);
        
        bool flag=treeTest.validateRBTree(nullptr);
        auto* one=treeTest.search(1);
        if(!flag){
            treeTest.printTree();
            cout<<"delete "<<insert_target[rand_num]<<" caused exception"<<endl;
            cout<<endl;
            return 1;
        }
        cout<<"--------------------------"<<endl;
    }

    //treeTest.printTree();

    treeTest.validateRBTree(nullptr);


    
    return 0;
}