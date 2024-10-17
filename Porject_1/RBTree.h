#ifndef RB_TREE_H
#define RB_TREE_H

#include "Utilility.h"

#include <functional>
#include <memory>

enum RBTreeColor { BLACKNODE, REDNODE };

template <typename KeyType, typename DataType>
struct RBTreeNode {
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

    const DataType &getData() const {
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

template <typename KeyType, typename DataType>
class RedBlackTree {
    using TreeNode = RBTreeNode<KeyType, DataType>;

  private:
    RBTreeNode<KeyType, DataType> *root;
    RBTreeNode<KeyType, DataType> *NULLNode;
    std::function<int(KeyType, KeyType)> compareTo;
    int size;

    /*检测是否所有红节点不连续出现*/
    bool validateNode(TreeNode *node) ;

    // 检测从当前节点到叶子节点的每条路径包含相同数量的黑色节点
    int validateBlackHeight(TreeNode *node);

    /*从node开始修复insert后的性质*/
    void insertFix(TreeNode *node);

    /*从node后开始修复delete后的性质*/
    void deleteFix(TreeNode *node);

    /*根据索引找到对应节点的递归体*/
    TreeNode *findByIndexHelper(TreeNode *node, int k) const;

    
    /*将v的子树替换到u的位置*/
    void transplant(TreeNode *u, TreeNode *v);

    /*找到node下最小的节点*/
    TreeNode *minimum(TreeNode *node);

    /*
    左旋操作
    @param
    node : 被旋转的节点
    */
    void leftRotate(TreeNode *node);

    /*
    右旋操作
    @param
    node : 被旋转的节点
    */
    void rightRotate(TreeNode *node);

    /*初始化哨兵节点*/
    void initNULLNode() {
        auto NULLNode_key = KeyType();
        auto NULLNode_data = DataType();
        NULLNode = new RBTreeNode<KeyType, DataType>(NULLNode_key, NULLNode_data, nullptr); // 默认构造
        NULLNode->setColor(BLACKNODE);
        this->NULLNode->NULLNode = this->NULLNode;
        NULLNode->left = this->NULLNode;
        NULLNode->right = this->NULLNode;
        root = nullptr;
    }

    /*统一的node构造方式*/
    TreeNode *createNode(const KeyType &key, DataType &data) {
        TreeNode *newNode = new TreeNode(key, data, NULLNode);
        return newNode;
    }

    /*维护所有节点的大小以及树的大小size值*/
     void updateSubtreeSize(TreeNode *node) {
        while (node != nullptr && node != NULLNode) {
            node->updateSize();
            node = node->parent;
        }
        size = root->subtreeSize;
    }

    /*删除包括node在内的子树*/
    void deleteTree(TreeNode *node) {
        if (node != NULLNode) {      
            deleteTree(node->left);  
            deleteTree(node->right); 
            delete node;             
        }
    }

    

  public:
   /*允许传入lambda函数自定义比较方式
     默认compareTo(key1,key2)的要求：
     key1<key2 返回小于0值
     key1==key2 返回等于0值
     Key1>key2 返回大于0的值
   */
    RedBlackTree(std::function<int(KeyType, KeyType)> comp) : compareTo(comp), size{0} {
        initNULLNode();
    }

    /*遵循RAII原则，释放资源*/
    ~RedBlackTree() {
        deleteTree(root);
        delete NULLNode;
    }

    RedBlackTree()
        : RedBlackTree([](KeyType a, KeyType b) {
              if (a > b) return 1;
              if (a == b) return 0;
              return -1;
          }) {
    }

    

    /*删除包括node在内的子树，默认从root开始*/
    void deleteTree() {
        deleteTree(root);
        root = nullptr;
        size = 0;
    }

    

    TreeNode *getRoot() const {
        return root;
    }

    TreeNode *getNullNode() const {
        return NULLNode;
    }

   
    /*根据index返回一个节点，index按照key的大小自动排列*/
    TreeNode *findByIndex(int k) const {
        return findByIndexHelper(root, k);
    }

    /*是否是空的*/
    bool isEmpty() const {
        return size == 0;
    }

    /*树的大小*/
    int getSize() const {
        return this->size;
    }
    
    /*打印树*/
    void printTree(TreeNode *node, std::string indent = "", bool isLeft = true) const;

    /*打印树*/
    void printTree(std::string indent = "", bool isLeft = true) const;

    /*根据key value插入一个节点*/
    TreeNode * insert(const KeyType &key, DataType data);

    /*根据key删除一个节点*/
    void deleteNode(const KeyType &key);

    /*根据key找到对应节点，如果有重复的，返回第一个，如果没有找到，返回哨兵节点*/
    TreeNode *search(const KeyType &key);

    /*得到key对应节点的index*/
    int indexOf(const KeyType &key) const;

    /*验证树的性质*/
    bool validateRBTree(TreeNode *node = nullptr);

    
};

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::leftRotate(TreeNode *node) {
    TreeNode *node_right = node->getRight();
    if (node_right == NULLNode) {
        return;
    }
    node->setRight(node_right->left);

    /*Step 1
        过继右左节点的所有权到自己
    */
    if (node_right->left != NULLNode) {
        node_right->left->setParent(node);
    }

    /*Step 2
    将右节点设置为新的r

    */

    node_right->setParent(node->parent);

    if (node->isRoot()) { // 根节点时的操作
        this->root = node_right;
    } else if (node->isLeftChild()) {
        node->parent->setLeft(node_right);
    } else {
        node->parent->setRight(node_right);
    }

    node_right->setLeft(node); // 转换父子关系
    node->setParent(node_right);
    node->updateSize();
    node_right->updateSize();
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::rightRotate(TreeNode *node) {
    TreeNode *node_left = node->left;
    if (node_left == NULLNode) {
        return;
    }
    node->setLeft(node_left->right);

    if (node_left->right != NULLNode) {
        node_left->right->setParent(node);
    }

    node_left->setParent(node->parent);

    if (node->isRoot()) {
        this->root = node_left;
    } else if (node->isRightChild()) {
        node->parent->setRight(node_left);
    } else {
        node->parent->setLeft(node_left);
    }

    node_left->setRight(node);
    node->updateSize();
    node_left->updateSize();
}

template <typename KeyType, typename DataType>
RBTreeNode<KeyType,DataType>* RedBlackTree<KeyType, DataType>::insert(const KeyType &key, DataType data) {
    if (search(key) != nullptr) {
        deleteNode(key);
    }
    TreeNode *z = createNode(key, data);

    TreeNode *y = nullptr;
    TreeNode *x = root;

    // 遍历树，找到插入点
    while (x != NULLNode && x != nullptr) { 
        y = x;
        if (compareTo(z->key, x->key) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == nullptr) {
        root = z;
    } else if (compareTo(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    this->updateSubtreeSize(z);

    insertFix(z);

    return z;
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::insertFix(TreeNode *node) {
    TreeNode *y;

    // 修复父节点为红色的情况
    while (node->getParent() != nullptr && node->getParent()->getColor() == REDNODE) {
        if (node->getGrandParent() != nullptr && node->getParent()->isLeftChild()) { // 父节点是祖父的左子节点
            y = node->getUncleAunt();                                                // 叔叔节点

            if (y != NULLNode && y->getColor() == REDNODE) { // Case 1: 叔叔是红色
                node->getParent()->setColor(BLACKNODE);
                y->setColor(BLACKNODE);
                node->getGrandParent()->setColor(REDNODE);
                node = node->getGrandParent(); // 向上修复
            } else {
                if (node->isRightChild()) { // Case 2: 节点是右子节点
                    node = node->getParent();
                    leftRotate(node); // 预旋转
                }

                // Case 3: 节点是左子节点
                node->getParent()->setColor(BLACKNODE);
                node->getGrandParent()->setColor(REDNODE);
                rightRotate(node->getGrandParent());
                break;
            }
        } else if (node->getGrandParent() != nullptr && node->getParent()->isRightChild()) { // 对称处理，父节点是祖父的右子节点
            y = node->getUncleAunt();                                                        // 叔叔节点

            if (y != NULLNode && y->getColor() == REDNODE) { // Case 1: 叔叔是红色
                node->getParent()->setColor(BLACKNODE);
                y->setColor(BLACKNODE);
                node->getGrandParent()->setColor(REDNODE);
                node = node->getGrandParent(); // 向上修复
            } else {
                if (node->isLeftChild()) { // Case 2: 节点是左子节点
                    node = node->getParent();
                    rightRotate(node); // 预旋转
                }

                // Case 3: 节点是右子节点
                node->getParent()->setColor(BLACKNODE);
                node->getGrandParent()->setColor(REDNODE);
                leftRotate(node->getGrandParent());
                break;
            }
        } else {
            break;
        }
    }

    // 确保根节点始终是黑色
    if (node == NULLNode) {
        node->parent = nullptr;
    }
    root->setColor(BLACKNODE);
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::deleteNode(const KeyType &key) {
    TreeNode *deleteTarget = search(key);
    if (!deleteTarget || deleteTarget->isNULLNode()) {
        return; // 空节点不进行删除操作
    }

    TreeNode *replaceNode;
    TreeNode *fixNode;
    RBTreeColor originalColor = deleteTarget->getColor(); // 保存删除节点的颜色
    if (!deleteTarget->hasLeft() && !deleteTarget->hasRight()) {
        replaceNode = deleteTarget->getRight();
        replaceNode->setParent(deleteTarget->getParent());
        transplant(deleteTarget, deleteTarget->getRight());
    } else if (!deleteTarget->hasLeft()) {
        // 目标节点没有左子节点，用右子节点代替
        replaceNode = deleteTarget->getRight();
        transplant(deleteTarget, deleteTarget->getRight());
    } else if (!deleteTarget->hasRight()) {
        // 目标节点没有右子节点，用左子节点代替
        replaceNode = deleteTarget->getLeft();
        transplant(deleteTarget, deleteTarget->getLeft());
    } else {
        // 目标节点有两个子节点，找到其后继节点
        TreeNode *successor = minimum(deleteTarget->getRight());
        originalColor = successor->getColor();
        replaceNode = successor->getRight();

        if (replaceNode->isNULLNode()) {
            replaceNode->setParent(successor->getParent()); 
        }

        if (successor->getParent() == deleteTarget) {
            replaceNode->setParent(successor);
        } else {
            transplant(successor, successor->getRight());
            successor->setRight(deleteTarget->getRight());
        }

        transplant(deleteTarget, successor);
        successor->setLeft(deleteTarget->getLeft());
        successor->getLeft()->setParent(successor);
        successor->setColor(deleteTarget->getColor());
    }
    if (replaceNode->isNULLNode()) {
        this->updateSubtreeSize(replaceNode->getParent());
    } else {
        this->updateSubtreeSize(replaceNode);
    }

    if (originalColor == BLACKNODE) {
        deleteFix(replaceNode);
    }
    delete deleteTarget;
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::deleteFix(TreeNode *node) {

    while (node != root && node->isBlack()) {
        if (node->isLeftChild()) {
            TreeNode *sibling = node->getSibling();
            if (sibling->isRed()) { 
                sibling->setColor(BLACKNODE);
                node->getParent()->setColor(REDNODE);
                leftRotate(node->getParent());
                sibling = node->getParent()->getRight();
            }

            if (sibling->getLeft()->isBlack() && sibling->getRight()->isBlack()) {
                sibling->setColor(REDNODE);
                node = node->getParent();
            } else {
                if (sibling->getRight()->isBlack()) {
                    sibling->getLeft()->setColor(BLACKNODE);
                    sibling->setColor(REDNODE);
                    rightRotate(sibling);
                    sibling = node->getParent()->getRight();
                }
                sibling->setColor(node->getParent()->getColor());
                node->getParent()->setColor(BLACKNODE);
                sibling->getRight()->setColor(BLACKNODE);
                leftRotate(node->getParent());
                node = root;
            }
        } else {
            TreeNode *sibling = node->getSibling();
            if (sibling->isRed()) {
                sibling->setColor(BLACKNODE);
                node->getParent()->setColor(REDNODE);
                rightRotate(node->getParent());
                sibling = node->getParent()->getLeft();
            }

            if (sibling->getLeft()->isBlack() && sibling->getRight()->isBlack()) {
                sibling->setColor(REDNODE);
                node = node->getParent();
            } else {
                if (sibling->getLeft()->isBlack()) {
                    sibling->getRight()->setColor(BLACKNODE);
                    sibling->setColor(REDNODE);
                    leftRotate(sibling);
                    sibling = node->getParent()->getLeft();
                }
                sibling->setColor(node->getParent()->getColor());
                node->getParent()->setColor(BLACKNODE);
                sibling->getLeft()->setColor(BLACKNODE);
                rightRotate(node->getParent());
                node = root;
            }
        }
    }
    node->setColor(BLACKNODE);
    NULLNode->setColor(BLACKNODE);
    NULLNode->setParent(nullptr);
}

template <typename KeyType, typename DataType>
RBTreeNode<KeyType, DataType> *RedBlackTree<KeyType, DataType>::search(const KeyType &key) {
    TreeNode *z = root;
    if (z == nullptr) {
        return nullptr;
    }

    while (z != NULLNode) {
        if (compareTo(key, z->key) == 0) {
            break;
        } else if (compareTo(key, z->key) < 0) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
    return z;
}

template <typename KeyType, typename DataType>
RBTreeNode<KeyType, DataType> *RedBlackTree<KeyType, DataType>::minimum(TreeNode *node) {
    if (node == NULLNode) {
        return node;
    }
    while (node->left != NULLNode) {
        node = node->left;
    }
    return node;
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::transplant(TreeNode *oldNode, TreeNode *newNode) {
    if (oldNode->isRoot()) {
        root = newNode;
    } else if (oldNode->isLeftChild()) {
        oldNode->getParent()->setLeft(newNode);
    } else {
        oldNode->getParent()->setRight(newNode);
    }
    if (newNode != NULLNode) {
        newNode->setParent(oldNode->getParent());
    }
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::printTree(TreeNode *node, std::string indent, bool isLeft) const {
    if (node != NULLNode) {

        std::cout << indent;

        if (isLeft) {
            std::cout << "L----"; 
        } else {
            std::cout << "R----"; 
        }

        std::string color = (node->getColor() == REDNODE) ? "RED" : "BLACK";
        std::cout << node->key << "(" << color << ")" << std::endl;

        indent += (isLeft ? "|     " : "      ");

        printTree(node->left, indent, true);

        printTree(node->right, indent, false);
    }
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::printTree(std::string indent, bool isLeft) const {
    printTree(root,indent,isLeft);
}

template <typename KeyType, typename DataType>
int RedBlackTree<KeyType, DataType>::indexOf(const KeyType &key) const {
    TreeNode *node = root; 
    int index = 0;        
    while (node != nullptr && node != NULLNode) {
        int leftSize = (node->left != nullptr && node->left != NULLNode) ? node->left->subtreeSize : 0;

        if (compareTo(key, node->key) < 0) {
            node = node->left;
        } else if (compareTo(key, node->key) == 0) {
            return index + leftSize;
        } else {
            index += leftSize + 1;
            node = node->right;
        }
    }
    return -1; 
}

template <typename KeyType, typename DataType>
RBTreeNode<KeyType, DataType> *RedBlackTree<KeyType, DataType>::findByIndexHelper(TreeNode *node, int k) const {
    if (node == NULLNode || node == nullptr) {
        return nullptr; // 如果节点为空，返回空指针
    }

    int leftSize = (node->left != nullptr && node->left != NULLNode) ? node->left->subtreeSize : 0;

    if (k < leftSize) {
        return findByIndexHelper(node->left, k);
    } else if (k == leftSize) {
        return node;
    } else {
        return findByIndexHelper(node->right, k - leftSize - 1);
    }
}

template <typename KeyType, typename DataType>
bool RedBlackTree<KeyType, DataType>::validateRBTree(TreeNode *node ) {
    if (root == NULLNode) {
        return true; // 空树是有效的红黑树
    }

    // 检查根节点是否为黑色
    if (root->getColor() != BLACKNODE) {
        std::cout << "根节点不是黑色！" << std::endl;
        return false;
    }

    auto flag = validateNode(root) && validateBlackHeight(root) != -1;
    if (flag) {
        cout << "满足红黑树性质" << endl;
    }

    return flag;
}

template <typename KeyType, typename DataType>
bool RedBlackTree<KeyType, DataType>::validateNode(TreeNode *node) {
    if (node == NULLNode) {
        return true; 
    }

    //验证子节点颜色
    if (node->getColor() == REDNODE) {
        if (node->left->getColor() != BLACKNODE || node->right->getColor() != BLACKNODE) {
            std::cout << "红色节点 " << node->key << " 的子节点不是黑色！" << std::endl;
            return false;
        }
    }


    return validateNode(node->left) && validateNode(node->right);
}

// 验证性质5：从当前节点到叶子节点的每条路径包含相同数量的黑色节点
template <typename KeyType, typename DataType>
int RedBlackTree<KeyType, DataType>::validateBlackHeight(TreeNode *node) {
    if (node == NULLNode) {
        return 1; // 叶子节点返回黑色高度 1
    }

    int leftBlackHeight = validateBlackHeight(node->left);
    int rightBlackHeight = validateBlackHeight(node->right);

    if (leftBlackHeight == -1 || rightBlackHeight == -1 || leftBlackHeight != rightBlackHeight) {
        std::cout << "节点 " << node->key << " 的左右子树的黑色高度不一致！" << "左为 " << leftBlackHeight << "右为 " << rightBlackHeight << std::endl;
        return -1;
    }


    return leftBlackHeight + (node->getColor() == BLACKNODE ? 1 : 0);
}
#endif
