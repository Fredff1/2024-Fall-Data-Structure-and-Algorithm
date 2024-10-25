#ifndef BTREEMAP_H
#define BTREEMAP_H
#include "BTree.h"
#include "Map.h"

template <typename KeyType, typename ValueType>
class BTreeMap : public Map<KeyType, ValueType> {
    using TreeNode = BTreeNode<KeyType, ValueType>;

  private:
    BTree<KeyType, ValueType> bTree;

  public:
    /*使用默认的比较器，要求KeyType重载> = <符号*/
    BTreeMap() : bTree(BTree<KeyType, ValueType>()) {
    }

    /*允许传入lambda函数自定义比较方式
     默认compareTo(key1,key2)的要求：
     key1<key2 返回小于0值
     key1==key2 返回等于0值
     Key1>key2 返回大于0的值
    */
    BTreeMap(std::function<int(KeyType, KeyType)> comp,int T=2) : bTree(BTree<KeyType, ValueType>(comp,T)) {
    }

    ~BTreeMap(){
        //bTree.~BTree();
    }

    /*插入一个key，value的节点*/
    void insert(const KeyType &key, const ValueType &value) override {
        this->bTree.insert(key, value);
    }

    /*
     根据key得到对应value的指针
     如果key不存在，返回nullptr
    */
    ValueType *get(const KeyType &key) override {
        TreeNode* node=bTree.search(key);
        if(node==nullptr){
            return nullptr;
        }
        auto& value=node->getValueByKey(key);
        return &value;
    }

    /*
    根据key设置对应的value为新value，如果key不存在，会创建一个新的节点
    */
    void set(const KeyType &key, const ValueType &value) override {
        TreeNode *node = bTree.search(key);
        if(node!=nullptr){
            int index=node->findKeyIndex(key);
            if(index!=-1){
                node->setValueAt(index,value);
            }
        }else{
            bTree.insert(key,value);
        }
    }

    /*根据index找到目标的value*/
   std::pair<KeyType&,ValueType&> findByIndex(const int index) override {
        if (index >= bTree.getSize()) {
            throw std::out_of_range("Index out of range");
        }
        return bTree.findNodeByIndex(index);
        
    }

    /*根据key返回value，如果value不存在，会添加一个默认的key-value键并返回*/
    ValueType &operator[](const KeyType &key) override {
        TreeNode *node = bTree.search(key);
        if(node==nullptr){
            bTree.insert(key,ValueType());
            node = bTree.search(key);
        }
        
        ValueType& value=node->getValueByKey(key);

        return value;
    }

    /*删除对应key-value对*/
    void erase(const KeyType &key) override {
        bTree.remove(key);
    }

    /*是否为空*/
    bool empty() const override {
        return bTree.getSize()==0;
    }

    /*清除所有项*/
    void clear() override {
        bTree.deleteAll();
    }

    /*得到对应key的node的index，不存在，返回-1*/
    int indexOf(const KeyType &key) const override {
        return bTree.findIndexByKey(key);
    }

    /*得到大小*/
    int size() const override {
        return bTree.getSize();
    }

    std::string toString(int indent) const override{
        return bTree.toStringRepresentation(indent);
    }

    void print(std::string indent = "  ") const {
        int indent_size=indent.size();
        bTree.printTree(indent_size);
    }
};

#endif 
