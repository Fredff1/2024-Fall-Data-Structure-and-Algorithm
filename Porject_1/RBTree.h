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
    RBTreeColor color;
    RBTreeNode *left;
    RBTreeNode *right;
    RBTreeNode *parent;
    RBTreeNode *NULLNode;

    RBTreeNode(KeyType key, DataType &data, RBTreeNode<KeyType, DataType> *NULLNode) : key(key), data(std::make_unique<DataType>(data)), color(REDNODE), left(NULLNode), right(NULLNode), NULLNode(NULLNode), parent(nullptr) {
    }

    ~RBTreeNode() {
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

    void insertFix(TreeNode *node);

    void deleteFix(TreeNode *node);

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

    void transplant(TreeNode *u, TreeNode *v);

    TreeNode *minimum(TreeNode *node);

    void deleteTree(TreeNode *node) {
        if (node != NULLNode) { // 递归删除左右子节点
            deleteTree(node->left);
            deleteTree(node->right);
            delete node; // 最后删除当前节点
        }
    }

  public:
    RedBlackTree(std::function<int(KeyType, KeyType)> comp) : compareTo(comp) {
        initNULLNode();
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete NULLNode;
    }

    TreeNode *createNode(const KeyType &key, DataType &data) {
        TreeNode *newNode = new TreeNode(key, data, NULLNode);
        return newNode;
    }

    RedBlackTree()
        : RedBlackTree([](KeyType a, KeyType b) {
              if (a > b) return 1;
              if (a == b) return 0;
              return -1;
          }) {
    }

    TreeNode *getRoot() const {
        return root;
    }
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

    void printTree(TreeNode *node, std::string indent = "", bool isLeft = true) const;

    void printTree(std::string indent = "", bool isLeft = true) const;

    void insert(const KeyType &key, DataType data);

    void deleteNode(const KeyType &key);

    TreeNode *search(const KeyType &key);

    bool validateRBTree(TreeNode *node = nullptr) {
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
        // 验证红黑树的所有性质
        return flag;
    }

    bool validateNode(TreeNode *node) {
        if (node == NULLNode) {
            return true; // 虚拟叶子节点，返回 true
        }

        // 性质4: 红色节点的子节点必须是黑色
        if (node->getColor() == REDNODE) {
            if (node->left->getColor() != BLACKNODE || node->right->getColor() != BLACKNODE) {
                std::cout << "红色节点 " << node->key << " 的子节点不是黑色！" << std::endl;
                return false;
            }
        }

        // 递归验证左右子树
        return validateNode(node->left) && validateNode(node->right);
    }

    // 验证性质5：从当前节点到叶子节点的每条路径包含相同数量的黑色节点
    int validateBlackHeight(TreeNode *node) {
        if (node == NULLNode) {
            return 1; // 叶子节点返回黑色高度 1
        }

        int leftBlackHeight = validateBlackHeight(node->left);
        int rightBlackHeight = validateBlackHeight(node->right);

        // 如果左右子树的黑色高度不相等，说明红黑树性质被破坏
        if (leftBlackHeight == -1 || rightBlackHeight == -1 || leftBlackHeight != rightBlackHeight) {
            std::cout << "节点 " << node->key << " 的左右子树的黑色高度不一致！" << "左为 " << leftBlackHeight << "右为 " << rightBlackHeight << std::endl;
            return -1;
        }

        // 如果当前节点是黑色，黑色高度加1
        return leftBlackHeight + (node->getColor() == BLACKNODE ? 1 : 0);
    }
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
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::insert(const KeyType &key, DataType data) {
    TreeNode *z = createNode(key, data);

    TreeNode *y = nullptr;
    TreeNode *x = root;

    // 遍历树，找到插入点
    while (x != NULLNode && x != nullptr) { // 不需要检查nullptr，统一使用NULLNode
        y = x;
        if (compareTo(z->key, x->key) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    // 设置新节点的父节点
    z->parent = y;

    // 如果父节点是NULLNode，说明插入的是根节点
    if (y == nullptr) {
        root = z;
    } else if (compareTo(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    // 调用插入修复函数来修正红黑树性质
    insertFix(z);
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

// template <typename KeyType, typename DataType>
// void RedBlackTree<KeyType, DataType>::deleteNode(const KeyType &key) {
//     TreeNode *x, *y;
//     TreeNode *deleteTarget = search(key);
//     if (deleteTarget->isNULLNode()) {
//         cout << "delete target does not exist" << endl;
//         return;
//     }

//     y = deleteTarget;
//     RBTreeColor originalColor = deleteTarget->getColor();

//     // 叶节点相当于对NULLnodetransplant，效果是一样的
//     if (!deleteTarget->hasLeft()) {
//         // 没有左节点
//         x = deleteTarget->getRight();
//         transplant(deleteTarget, deleteTarget->right);
//     } else if (!deleteTarget->hasRight()) {
//         x = deleteTarget->getLeft();
//         transplant(deleteTarget, deleteTarget->left);
//     } else {
//         y = minimum(deleteTarget->right);
//         originalColor = y->getColor();
//         x = y->getRight();
//         if (y->getParent() != deleteTarget) {
//             transplant(y, y->getRight());
//             y->setRight(deleteTarget->right);
//         }
//         transplant(deleteTarget, y);
//         y->setLeft(deleteTarget->left);

//         y->setColor(deleteTarget->getColor());
//     }

//     if (originalColor == BLACKNODE) {
//         if(x->isNULLNode()){
//             x->setParent(deleteTarget->getParent());
//         }
//         deleteFix(x);
//     }

//     delete deleteTarget;
//     cout<<"delete success"<<endl;
// }

// template <typename KeyType, typename DataType>
// void RedBlackTree<KeyType, DataType>::deleteFix(TreeNode* node) {
//     while (node != root && node->getColor() == BLACKNODE) {
//         if (node == node->getParent()->left) {
//             TreeNode* sibling = node->getParent()->right;

//             // Case 1: Sibling is red
//             if (sibling->getColor() == REDNODE) {
//                 sibling->setColor(BLACKNODE);
//                 node->getParent()->setColor(REDNODE);
//                 leftRotate(node->getParent());
//                 sibling = node->getParent()->right;
//             }

//             // Case 2: Sibling is black, and both of sibling's children are black or NULLNode
//             if (sibling->getLeft()->getColor() == BLACKNODE && sibling->getRight()->getColor() == BLACKNODE) {
//                 sibling->setColor(REDNODE);
//                 node = node->getParent();
//             } else {
//                 // Case 3: Sibling is black and sibling's right child is black, left child is red
//                 if (sibling->getRight()->getColor() == BLACKNODE) {
//                     sibling->getLeft()->setColor(BLACKNODE);
//                     sibling->setColor(REDNODE);
//                     rightRotate(sibling);
//                     sibling = node->getParent()->right;
//                 }

//                 // Case 4: Sibling is black, and sibling's right child is red
//                 sibling->setColor(node->getParent()->getColor());
//                 node->getParent()->setColor(BLACKNODE);
//                 sibling->getRight()->setColor(BLACKNODE);
//                 leftRotate(node->getParent());
//                 node = root;
//             }
//         } else {
//             // 对称的情况，处理右子节点的情况
//             TreeNode* sibling = node->getParent()->left;

//             // Case 1: Sibling is red
//             if (sibling->getColor() == REDNODE) {
//                 sibling->setColor(BLACKNODE);
//                 node->getParent()->setColor(REDNODE);
//                 rightRotate(node->getParent());
//                 sibling = node->getParent()->left;
//             }

//             // Case 2: Sibling is black, and both of sibling's children are black or NULLNode
//             if (sibling->getLeft()->getColor() == BLACKNODE && sibling->getRight()->getColor() == BLACKNODE) {
//                 sibling->setColor(REDNODE);
//                 node = node->getParent();
//             } else {
//                 // Case 3: Sibling is black and sibling's left child is black, right child is red
//                 if (sibling->getLeft()->getColor() == BLACKNODE) {
//                     sibling->getRight()->setColor(BLACKNODE);
//                     sibling->setColor(REDNODE);
//                     leftRotate(sibling);
//                     sibling = node->getParent()->left;
//                 }

//                 // Case 4: Sibling is black, and sibling's left child is red
//                 sibling->setColor(node->getParent()->getColor());
//                 node->getParent()->setColor(BLACKNODE);
//                 sibling->getLeft()->setColor(BLACKNODE);
//                 rightRotate(node->getParent());
//                 node = root;
//             }
//         }
//     }
//     node->setColor(BLACKNODE);  // 无论如何，最终将node设为黑色
// }

// template <typename KeyType, typename DataType>
// void RedBlackTree<KeyType, DataType>::deleteFix(TreeNode *node) {
//     TreeNode* w;

//     while(node!=root &&node->isBlack()){
//         if(node->isLeftChild()){
//             w=node->getSibling();
//             if(w->isRed()){//case 1
//                 w->setColor(BLACKNODE);
//                 node->getParent()->setColor(REDNODE);
//                 leftRotate(node->getParent());
//                 w=node->parent->getRight();
//             }
//             if(w->isNULLNode()==false&&w->getLeft()->isBlack() && w->getRight()->isBlack()){// case 2
//                 w->setColor(REDNODE);
//                 node=node->getParent();
//             }else if(w->isNULLNode()==false&&w->getRight()&&w->getRight()->isBlack()){// case 3
//                 w->getLeft()->setColor(BLACKNODE);
//                 w->setColor(REDNODE);
//                 rightRotate(w);
//                 w=node->getParent()->getRight();
//             }
//             w->setColor(node->getParent()->getColor()); // case 4
//             node->getParent()->setColor(BLACKNODE);
//             if(w->isNULLNode()==false){
//                 w->getRight()->setColor(BLACKNODE);
//             }

//             leftRotate(node->getParent());
//             node=root;

//         }else{
//             // 对称处理: 当 node 是右子节点
//             w = node->getSibling();  // 兄弟节点
//             if (w->isRed()) {  // case 1: 兄弟节点 w 是红色
//                 w->setColor(BLACKNODE);
//                 node->getParent()->setColor(REDNODE);
//                 rightRotate(node->getParent());
//                 w = node->getParent()->getLeft();
//             }
//             if (w->isNULLNode()==false&&w->getRight()->isBlack() && w->getLeft()->isBlack()) {  // case 2: w 的两个子节点都是黑色
//                 w->setColor(REDNODE);
//                 node = node->getParent();
//             } else {
//                 if (w->isNULLNode()==false&&w->getLeft()->isBlack()) {  // case 3: w 的左子节点是黑色
//                     w->getRight()->setColor(BLACKNODE);
//                     w->setColor(REDNODE);
//                     leftRotate(w);
//                     w = node->getParent()->getLeft();
//                 }
//                 // case 4: w 的左子节点是红色
//                 w->setColor(node->getParent()->getColor());
//                 node->getParent()->setColor(BLACKNODE);
//                 if(w->isNULLNode()==false){
//                     w->getLeft()->setColor(BLACKNODE);
//                 }

//                 rightRotate(node->getParent());
//                 node = root;
//             }
//         }
//     }
//     node->setColor(BLACKNODE);
// }
template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::deleteNode(const KeyType &key) {
    TreeNode *deleteTarget = search(key);
    if (!deleteTarget || deleteTarget->isNULLNode()) {
        return; // 空节点不进行删除操作
    }

    TreeNode *replaceNode;
    TreeNode *fixNode;
    RBTreeColor originalColor = deleteTarget->getColor(); // 保存删除节点的颜色
    if(!deleteTarget->hasLeft()&&!deleteTarget->hasRight()){
        replaceNode = deleteTarget->getRight();
        replaceNode->setParent(deleteTarget->getParent());
        transplant(deleteTarget, deleteTarget->getRight());
        
    }
    if (!deleteTarget->hasLeft()) {
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
            replaceNode->setParent(successor->getParent());  // NULLNode 设置父节点
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

    // 如果删除的是黑色节点，需要调用 deleteFix 进行调整
    if (originalColor == BLACKNODE) {
       
        deleteFix(replaceNode);
    }
    delete deleteTarget;
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::deleteFix(TreeNode *node) {

    while (node != root && node->isBlack()) {
        if (node->isLeftChild()) {// 左儿子情况
            TreeNode *sibling = node->getSibling();
            if (sibling->isRed()) { // 兄弟是红色
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
        } else { // Symmetric case for the right child
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
}

template <typename KeyType, typename DataType>
RBTreeNode<KeyType, DataType> *RedBlackTree<KeyType, DataType>::search(const KeyType &key) {
    TreeNode *z = root;
    // 查找要删除的节点 z
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
    if (node != NULLNode) { // 使用 NULLNode 代替 nullptr
        // 打印缩进
        std::cout << indent;

        if (isLeft) {
            std::cout << "L----"; // 当前节点是左子节点
        } else {
            std::cout << "R----"; // 当前节点是右子节点
        }

        // 打印节点的键值和颜色
        std::string color = (node->getColor() == REDNODE) ? "RED" : "BLACK";
        std::cout << node->key << "(" << color << ")" << std::endl;

        // 调整缩进量来显示子节点层次结构
        indent += (isLeft ? "|     " : "      ");

        // 递归打印左子树
        printTree(node->left, indent, true);

        // 递归打印右子树
        printTree(node->right, indent, false);
    }
}

template <typename KeyType, typename DataType>
void RedBlackTree<KeyType, DataType>::printTree(std::string indent, bool isLeft) const {
    TreeNode *node = this->root;
    if (node != NULLNode) { // 使用 NULLNode 代替 nullptr
        // 打印缩进
        std::cout << indent;

        if (isLeft) {
            std::cout << "L----"; // 当前节点是左子节点
        } else {
            std::cout << "R----"; // 当前节点是右子节点
        }

        // 打印节点的键值和颜色
        std::string color = (node->getColor() == REDNODE) ? "RED" : "BLACK";
        std::cout << node->key << "(" << color << ")" << std::endl;

        // 调整缩进量来显示子节点层次结构
        indent += (isLeft ? "|     " : "      ");

        // 递归打印左子树
        printTree(node->left, indent, true);

        // 递归打印右子树
        printTree(node->right, indent, false);
    }
}

#endif
