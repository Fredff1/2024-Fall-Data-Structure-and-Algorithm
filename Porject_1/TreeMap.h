#ifndef TREEMAP_H
#define TREEMAP_H
#include "RBTree.h"

template <typename KeyType, typename ValueType>
class TreeMap{
    using TreeNode=RBTreeNode<KeyType,ValueType>;
  private:
    RedBlackTree<KeyType,ValueType> rbTree;

  public:
    /*使用默认的比较器，要求KeyType重载> = <符号*/
    TreeMap():rbTree(RedBlackTree<KeyType,ValueType>()){}

    /*允许传入lambda函数自定义比较方式
     默认compareTo(key1,key2)的要求：
     key1<key2 返回小于0值
     key1==key2 返回等于0值
     Key1>key2 返回大于0的值
    */
    TreeMap(std::function<int(KeyType, KeyType)> comp):rbTree(RedBlackTree<KeyType,ValueType>(comp)){

    }
    

    
    /*插入一个key，value的节点*/
    TreeNode* insert(const KeyType& key,const ValueType& value){
        TreeNode* node=this->rbTree.insert(key,value);
        return node;
    }

    /*
     根据key得到对应value的指针 
     如果key不存在，返回nullptr
    */
    ValueType* get(const KeyType& key) {
        TreeNode* node = rbTree.search(key);
        if (node == nullptr || node->isNULLNode()) {
            return nullptr;
        }
        return (node->data.get());
    }

    /*
    根据key设置对应的value为新value，如果key不存在，会创建一个新的节点
    */
    void set(const KeyType& key,const ValueType& value){
        TreeNode* node = rbTree.search(key);
        if (node == nullptr || node->isNULLNode()) {
            insert(key,value);
        }else{
            node->setData(value);
        }
    }

    /*根据index找到目标的value*/
    const ValueType& find(const int index){
        if(index>=rbTree.getSize()){
            throw std::out_of_range("Index out of range");
        }
        TreeNode* node=rbTree.findByIndex(index);
        return node->getData();
    }

    /*根据key返回value，如果value不存在，会添加一个默认的key-value键并返回*/
     ValueType& operator[](const KeyType& key) {
        TreeNode* node = rbTree.search(key);

        if (!node || node->isNULLNode()) {
            node=rbTree.insert(key, ValueType());  
        }

        return *(node->data.get());
    }

    /*删除对应key-value对*/
    void erase(const KeyType& key) {
        rbTree.deleteNode(key);
    }

    /*是否为空*/
    bool empty() const{
        return rbTree.isEmpty();
    }

    /*清除所有项*/
    void clear(){
        rbTree.deleteTree();

    }

    /*得到对应key的node的index，不存在，返回-1*/
    int indexOf(const KeyType& key) const{
        return rbTree.indexOf(key);
    }

    /*得到大小*/
    int size() const{
        return rbTree.getSize();
    }
};


#endif 
