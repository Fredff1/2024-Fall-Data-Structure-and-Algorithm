#ifndef TREENODE_H
#define TREENODE_H
#include "Utilility.h"

#include <memory>


template<typename KeyType,typename ValueType>
struct MyTreeMode{
   


};

enum RBTreeColor { BLACKNODE, REDNODE };

template <typename KeyType, typename DataType>
struct RBTreeNode :public MyTreeMode<KeyType,DataType>{
    KeyType key;
    std::unique_ptr<DataType> data;
    int subtreeSize;
    RBTreeColor color;
    RBTreeNode *left;
    RBTreeNode *right;
    RBTreeNode *parent;
    RBTreeNode *NULLNode;

    RBTreeNode(KeyType key, DataType &data, RBTreeNode<KeyType, DataType> *NULLNode) : key(key), data(std::make_unique<DataType>(data)), color(REDNODE), left(NULLNode), right(NULLNode), NULLNode(NULLNode), parent(nullptr), subtreeSize(0) {
    }

    ~RBTreeNode() {
    }

    /*维护节点的subTreeSIze信息*/
    void updateSize() {
        subtreeSize = 1;
        if (left) {
            subtreeSize += left->subtreeSize;
        }
        if (right) {
            subtreeSize += right->subtreeSize;
        }
    }

    DataType &getData(){
        return *data;
    }

    void setData(DataType &newData) {
        data = std::move(std::make_unique<DataType>(newData));
    }

    inline bool isNULLNode() const {
        return this == NULLNode;
    }

    inline void shiftColor() {
        if (this->color == BLACKNODE) {
            this->color = REDNODE;
        } else {
            this->color = BLACKNODE;
        }
    }

    inline void setColor(RBTreeColor c) {
        this->color = c;
    }

    inline RBTreeColor getColor() const {
        return this->color;
    }

    inline bool isBlack() const {
        return color == BLACKNODE;
    }

    inline bool isRed() const {
        return color == REDNODE;
    }

    inline bool isRoot() const {
        return parent == nullptr;
    }

    inline bool isLeftChild() const {
        return parent != nullptr && parent->left == this;
    }

    inline bool isRightChild() const {
        return parent != nullptr && parent->right == this;
    }

    inline bool hasLeft() const {
        return left != nullptr && left != NULLNode;
    }

    inline bool hasRight() const {
        return right != nullptr && right != NULLNode;
    }

    inline bool hasChild() const {
        return (!hasLeft()) && (!hasRight());
    }

    inline RBTreeNode *getGrandParent() {
        if (parent != nullptr) {
            return parent->getParent();
        }
        return NULLNode;
    }

    inline RBTreeNode *getParent() {
        return this->parent;
    }

    void setParent(RBTreeNode *node) {
        this->parent = node;
    }

    inline RBTreeNode *getUncleAunt() {
        if (parent == nullptr) {
            return NULLNode;
        }
        RBTreeNode *grand_parent = getGrandParent();
        if (!grand_parent) {
            return NULLNode;
        }
        if (parent->isLeftChild()) {
            return grand_parent->right;
        } else {
            return grand_parent->left;
        }
    }

    inline RBTreeNode *getSibling() {
        if (parent == nullptr) {
            return NULLNode;
        }

        if (isLeftChild()) {
            return parent->right;
        } else {
            return parent->left;
        }
    }

    inline void setSibling(RBTreeNode *node) {
        if (parent == nullptr) {
            return;
        }
        if (isLeftChild()) {
            parent->right = node;
        } else {
            parent->left = node;
        }
    }

    inline RBTreeNode *getLeft() {
        return this->left;
    }

    inline RBTreeNode *getRight() {
        return this->right;
    }

    inline void setLeft(RBTreeNode *node) {
        if (!node) {
            return;
        }
        this->left = node;
        if (node != NULLNode) { // 避免修改 NULLNode 的父节点
            node->parent = this;
        }
    }

    inline void setRight(RBTreeNode *node) {
        if (!node) {
            return;
        }
        this->right = node;
        if (node != NULLNode) {
            node->parent = this;
        }
    }
};


template<typename KeyType,typename ValueType>
struct BTreeNode{
    std::vector<KeyType> keys;
    std::vector<ValueType> values;
    std::vector<BTreeNode*> children;
    bool isLeafNode;
    int keyNums;
    int childNums;
    int T;
    BTreeNode(bool leaf,int T):T(T) {
        isLeafNode = leaf;
        keyNums = 0;
        childNums=0;
        keys.resize(2 * T - 1);   
        values.resize(2*T-1);      // 节点中最多能存储 2T-1 个键值
        children.resize(2 * T-1);         // 最多有 2T-1 个子节点
    }

    bool isFull() const {
        return keyNums>=(2*T-1);
    }

    void setKeyValueAt(int index, const KeyType& key, const ValueType& value){
        keys[index]=key;
        values[index]=value;
    }

    KeyType& getKeyAt(int index){
        if(index < 0 || index >= keyNums){
            throw std::invalid_argument("Index cannot be bigger than T");
        }
        return keys[index];
    }

    BTreeNode* getChildAt(int index){
        if(index < 0 || index >= childNums){
            throw std::invalid_argument("Index cannot be bigger than T");
        }
        return children[index];
    }

    ValueType& getValueAt(int index) {
        if (index < 0 || index >= keyNums) {
            throw std::invalid_argument("Index exceeds bounds of value array");
        }
        return values[index];
    }

    void insertKeyValueAt(int index, const KeyType& key, const ValueType& value) {
        if (index < 0 || index > keyNums || index >= keys.size()) {
            throw std::invalid_argument("Invalid index for insertion");
        }


        // 步骤2：右移所有比 index 后的键
        for (int i = keyNums - 1; i >= index; i--) {
            keys[i + 1] = keys[i];  // 将现有键值右移一位
            values[i + 1] = values[i];
        }

        // 步骤3：在 index 位置插入新键
        keys[index] = key;
        values[index] = value;
        keyNums++;  // 增加节点中键的数量
    }



    void insertChildAt(int index, BTreeNode* child) {
        if (index < 0 || index > 2*T ) {
            throw std::invalid_argument("Invalid index for child insertion");
        }


        // 步骤2：右移所有比 index 后的子节点
        for (int i = childNums; i > index; i--) {
            children[i] = children[i - 1];  // 将现有子节点右移
        }

        // 步骤3：在 index 位置插入新子节点
        children[index] = child;
        childNums++;  // 增加子节点的数量
    }


    void removeKeyValueAt(int index) {
        if (index < 0 || index > keyNums ) {
            throw std::invalid_argument("Invalid index for removal");
        }


        for (int i = index; i < keyNums - 1; i++) {
            keys[i] = keys[i + 1];
            values[i] = values[i + 1];
        }
        keyNums--;
        
    }


    void removeChildAt(int index) {
        if (index < 0 || index > childNums ) {
            throw std::invalid_argument("Invalid index for child removal");
        }

        for (int i = index; i < childNums-1; i++) {
            children[i] = children[i + 1];
        }
        childNums--;
    }

    void setIsLeaf(bool flag){
        this->isLeafNode=flag;
    }

    bool isLeaf() const{
        return isLeafNode;
    }

    int getT() const{
        return this->T;
    }

    int getKeyNums() const{
        return keyNums;
    }

    int getChildNums() const{
        return childNums;
    }
};

#endif 
