#include "BTree.h"
#include "Utilility.h"

void test_1(){
    BTree<int,int> testTree=BTree<int,int>(2);
    auto a=RandomGenerator::getRandomVector<int>(10000,0,10000000);
    for(auto&va:a){
        testTree.insert(va,va);
    }
    auto flag1=testTree.validateBTree();

    if(flag1){
        cout<<"insert验证通过"<<endl;
    }
    for(int i=0;i<a.size();i+=2){
        testTree.remove(i);
    }
    flag1=testTree.validateBTree();
    if(flag1){
        cout<<"delete验证通过"<<endl;
    }
}

void test_2(){
BTree<char,char> testTree2=BTree<char,char>(2);
    char b[100]={'F' ,'S', 'Q', 'K', 'C', 'L', 'H', 'T', 'V', 'W', 'M', 'R', 'N', 'P', 'A', 'B','X', 'Y', 'D', 'Z', 'E'};
    for(int i=0;i<21;i++){
        char ch=b[i];
        cout<<"---------------------------------------------\n";
        cout<<"Insert "<<ch<<endl;
        testTree2.insert(ch,ch);
        //testTree2.printTree();
        testTree2.validateBTree();
        cout<<"---------------------------------------------\n";
    }
    
    auto flag=testTree2.validateBTree();
    if(flag){
        cout<<"验证通过"<<endl;
    }

    testTree2.printTree();

    char b_remove[]={'B','C','M','K','L','R'};
    for(int j=0;j<6;j++){
        cout<<"---------------------------------------------\n";
        cout<<"Remove "<<b_remove[j]<<endl;
        testTree2.remove(b_remove[j]);
        testTree2.printTree();
        testTree2.validateBTree();
        cout<<"---------------------------------------------\n";
    }
    
    testTree2.printTree();
}

int main(){
    test_1();
    //test_2();

    
    return 0;
}