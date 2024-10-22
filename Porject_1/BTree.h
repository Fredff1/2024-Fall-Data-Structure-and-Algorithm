#ifndef B_TREE_H
#define B_TREE_H
#include "TreeNode.h"
#include <functional>
#include <memory>
#include <set>
#include <stdexcept>

template <typename KeyType, typename ValueType>
class BTree {
    using TreeNode = BTreeNode<KeyType, ValueType>;

  private:
    TreeNode *root;

    int T = 5;

    std::function<int(KeyType, KeyType)> compareTo;

    TreeNode *search(TreeNode *node, KeyType key);

    void splitChild(TreeNode *parent, int i, TreeNode *fullChild);

    void insertNotFull(TreeNode *node, KeyType key, ValueType &value);

    void removeHelper(TreeNode *node, KeyType key);

    KeyType getPredecessor(TreeNode *node);

    KeyType getSuccessor(TreeNode *node);

    void mergeNodes(TreeNode *parent, int index);

    void borrowFromPrev(TreeNode *node, int index);

    void borrowFromNext(TreeNode *node, int index);

    TreeNode *createNode(bool isLeaf) {
        TreeNode *newNode = new TreeNode(isLeaf, T);
        return newNode;
    }

    void fixUnderFlow(TreeNode *node,int index);

    void printTreeHelper(TreeNode *node, int depth, bool isLastChild) const;

    void deleteFromNonLeaf(TreeNode *node, int index);

  public:
    BTree(std::function<int(KeyType, KeyType)> comp, int T = 5) : T(T), root(nullptr), compareTo(comp) {
    }

    BTree(int T = 5)
        : BTree(
              [](KeyType a, KeyType b) {
                  if (a > b) return 1;
                  if (a == b) return 0;
                  return -1;
              },
              T) {
    }

    void insert(KeyType key, ValueType &value);

    // 删除键值
    void remove(KeyType key);

    TreeNode *search(KeyType key) {
        if (root == nullptr)
            return nullptr;
        else
            return search(root, key);
    }

    void printTree(int indent = 2) {
        if (root != nullptr) {
            printTreeHelper(root, 0, false); // 从根节点开始，初始深度为 0
        }
    }

    bool validateBTreeNode(TreeNode *node, int min_keys, int max_keys, int current_depth, std::set<int> &leaf_depths);

    bool validateBTree() {
        // 检查根节点是否为空
        if (root == nullptr) {
            std::cerr << "B树为空" << std::endl;
            return true; // 空树是合法的
        }

        // 创建一个集合用于存储叶节点的深度
        std::set<int> leaf_depths;

        // 根节点的键值数量可以少于T - 1，但不超过2*T - 1
        return validateBTreeNode(root, 1, 2 * T - 1, 0, leaf_depths);
    }
};

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::remove(KeyType key) {
    if (root == nullptr) {
        return;
    }

    removeHelper(root, key);
    if (root->getKeyNums() == 0) {
        if (root->isLeaf()) {
            root = nullptr;
        } else {
            TreeNode *oldRoot = root;
            root = root->getChildAt(0);
            delete oldRoot;
        }
    }
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::fixUnderFlow(TreeNode *node,int index){
        if (index != 0 && node->getChildAt(index - 1)->getKeyNums() >= T) {
            // 从前一个兄弟节点借位
            borrowFromPrev(node, index);
        } else if (index != node->getKeyNums() && node->getChildAt(index + 1)->getKeyNums() >= T) {
            // 从后一个兄弟节点借位
            borrowFromNext(node, index);
        } else {
            // 如果左右兄弟都不能借位，则合并
            if (index != node->getKeyNums()) {
                mergeNodes(node, index);
            } else {
                mergeNodes(node, index - 1);
            }
        }

    }
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::removeHelper(TreeNode *node, KeyType key) {
    int index = 0;
    while (index < node->getKeyNums() && compareTo(node->getKeyAt(index), key) < 0) {
        index++;
    }

    // case 1 找到了对应的节点
    if (index < node->getKeyNums() &&compareTo(node->getKeyAt(index), key) == 0) {
        if (node->isLeaf()) {
            // 直接删
            node->removeKeyValueAt(index);
            
        } else {
            // 删非叶节点
            deleteFromNonLeaf(node,index);
        }
    } else {
        if (node->isLeaf()) {
            // 节点不存在
            return;
            // 去子节点找
        } else {
            // 标记递归调用的子节点是否需要调整
            bool flag = (index == node->getKeyNums());

            // 如果子节点的键值数小于 T，进行修复
            if (node->getChildAt(index)->getKeyNums() < T) {
                fixUnderFlow(node, index);
            }

            // 递归删除，若发生了修复，可能需要在之前的子节点递归
            if (flag && index > node->getKeyNums()) {
                removeHelper(node->getChildAt(index - 1), key);
            } else {
                removeHelper(node->getChildAt(index), key);
            }
        }
    }

}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::deleteFromNonLeaf(TreeNode *node, int index) {
    KeyType key = node->getKeyAt(index);

    // 情况 1：如果前驱节点的键值数大于等于 T,操作左子树
    if (node->getChildAt(index)->getKeyNums() >= T) {
        KeyType pred = getPredecessor(node->getChildAt(index));
        node->setKeyValueAt(index, pred, node->getValueAt(index));
        removeHelper(node->getChildAt(index), pred);
    }
    // 情况 2：如果后继节点的键值数大于等于 T，操作右子树
    else if (node->getChildAt(index + 1)->getKeyNums() >= T) {
        KeyType succ = getSuccessor(node->getChildAt(index + 1));
        node->setKeyValueAt(index, succ, node->getValueAt(index));
        removeHelper(node->getChildAt(index + 1), succ);
    }
    // 情况 3：如果前驱和后继节点的键值数都小于 T
    else {
         mergeNodes(node, index);  // 合并左右子树
         removeHelper(node->getChildAt(index), key);  // 递归删除
    }
}

/*合并index和Index+1的child*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::mergeNodes(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);//left child
    TreeNode *sibling = node->getChildAt(index + 1); //right child

    // 将 parent 的键值下移到 child，parent->keys[index] 被删除
    child->insertKeyValueAt(child->getKeyNums(), node->getKeyAt(index), node->getValueAt(index));

    // 将 sibling 的键值和子节点合并到 child 中
    for (int i = 0; i < sibling->getKeyNums(); i++) {
        child->insertKeyValueAt(child->getKeyNums(), sibling->getKeyAt(i), sibling->getValueAt(i));
    }

    if(!sibling->isLeaf()){
        for (int i = 0; i < sibling->getChildNums(); i++) {
            child->insertChildAt(child->getChildNums(), sibling->getChildAt(i));
        }
    }

   

    // 将 parent 的键值左移，删除 parent->keys[index]
    node->removeKeyValueAt(index);

    // 删除 sibling 子节点
    node->removeChildAt(index + 1);

    
}

/**从左兄弟借一个节点
parameters : index:借节点child在node的索引
*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::borrowFromPrev(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);          // 当前需要借键的子节点
    TreeNode *sibling = node->getChildAt(index - 1);    // 左兄弟节点

    // 将 parent 的键值下移到 child 第一个位置
    child->insertKeyValueAt(0, node->getKeyAt(index - 1), node->getValueAt(index - 1));

    // 更新 parent 的键值，使用左兄弟的最后一个键值
    node->setKeyValueAt(index - 1, sibling->getKeyAt(sibling->getKeyNums() - 1), sibling->getValueAt(sibling->getKeyNums() - 1));

    // 如果左兄弟节点不是叶节点，则移动其最后一个子节点到 child
    if (!sibling->isLeaf()) {
        child->insertChildAt(0, sibling->getChildAt(sibling->getChildNums() - 1));
        sibling->removeChildAt(sibling->getChildNums() - 1);  // 删除左兄弟的最后一个子节点
    }

    // 删除左兄弟的最后一个键值
    sibling->removeKeyValueAt(sibling->getKeyNums() - 1);
}

/*从右兄弟借一个节点*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::borrowFromNext(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);          // 当前需要借键的子节点
    TreeNode *sibling = node->getChildAt(index + 1);    // 右兄弟节点

    // 将 parent 的键值插入到 child 的末尾
    child->insertKeyValueAt(child->getKeyNums(), node->getKeyAt(index), node->getValueAt(index));

    // 更新 parent 的键值，用右兄弟的第一个键值
    node->setKeyValueAt(index, sibling->getKeyAt(0), sibling->getValueAt(0));

    // 如果右兄弟节点不是叶节点，将其第一个子节点移到 child
    if (!sibling->isLeaf()) {
        child->insertChildAt(child->getChildNums(), sibling->getChildAt(0));
        sibling->removeChildAt(0);  // 删除右兄弟的第一个子节点
    }

    // 删除右兄弟的第一个键值
    sibling->removeKeyValueAt(0);
}

template <typename KeyType, typename ValueType>
BTreeNode<KeyType, ValueType> *BTree<KeyType, ValueType>::search(TreeNode *node, KeyType key) {
    int i = 0;

    // 找到第一个大于或等于 key 的位置
    while (i < node->getKeyNums() && compareTo(node->getKeyAt(i), key) < 0) {
        i++;
    }

    // 如果 key 与 node->keys[i] 相等，返回当前节点
    if (i < node->getKeyNums() && compareTo(node->getKeyAt(i), key) == 0) {
        return node;
    }

    // 如果是叶节点，说明 key 不在树中
    if (node->isLeaf()) {
        return nullptr;
    }

    // 否则，递归地在子节点中继续查找
    return search(node->getChildAt(i), key);
}

template <typename KeyType, typename ValueType>
KeyType BTree<KeyType, ValueType>::getPredecessor(TreeNode *node) {
    // 移动到左子树中的最右节点（最大值）
    TreeNode *current = node;
    while (!current->isLeaf()) {
        current = current->getChildAt(current->getKeyNums()); // 最右子节点
    }
    return current->getKeyAt(current->getKeyNums() - 1); // 返回最后一个键
}

template <typename KeyType, typename ValueType>
KeyType BTree<KeyType, ValueType>::getSuccessor(TreeNode *node) {
    TreeNode *current = node;
    while (!current->isLeaf()) {
        current = current->getChildAt(0);
    }
    return current->getKeyAt(0);
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::splitChild(TreeNode *parent, int i, TreeNode *fullChild) {

    // 创建一个新的节点，保存 fullChild 的右半部分
    TreeNode *newChild = createNode(fullChild->isLeaf());

    // Step I 将 fullChild 的右侧键值对移动到 newChild
    for (int j = 0; j < T - 1; j++) {
        newChild->insertKeyValueAt(j, fullChild->getKeyAt(j + T), fullChild->getValueAt(j + T));
    }

    // Step II 如果 fullChild 不是叶节点，将 fullChild 的后 T 个子节点移动到 newChild
    if (!fullChild->isLeaf()) {
        for (int j = 0; j < T; j++) {
            newChild->insertChildAt(j, fullChild->getChildAt(j + T));
        }
    }

    // Step III 将 fullChild 的中间键值提升到 parent 中
    parent->insertKeyValueAt(i, fullChild->getKeyAt(T - 1), fullChild->getValueAt(T - 1));

    // 更新 fullChild 中键值数量，不需要循环删除，直接减少 keyNums
    fullChild->keyNums = T - 1;
    if (!fullChild->isLeaf()) {
        fullChild->childNums = T; // 剩下的 T 个子节点
    }

    // 将 newChild 插入到 parent 的 i+1 位置
    parent->insertChildAt(i + 1, newChild);
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::insert(KeyType key, ValueType &value) {
    if (root == nullptr) {
        root = createNode(true);
        root->insertKeyValueAt(0, key, value);
        return;
    } else {
        bool isRootFull = root->isFull();
        if (isRootFull) { // case 1
            TreeNode *newRoot = createNode(false);
            newRoot->insertChildAt(0, root);

            splitChild(newRoot, 0, root); // newRoot是新root，且原来的root左，令创建一个节点右
            int i = 0;
            if (newRoot->getKeyAt(0) < key) {
                i++;
            }
            insertNotFull(newRoot->getChildAt(i), key, value);

            root = newRoot;
        } else {
            // case 2 如果根节点未满，直接插入
            insertNotFull(root, key, value);
        }
    }
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::insertNotFull(TreeNode *node, KeyType key, ValueType &value) {

    int i = node->getKeyNums() - 1;

    if (node->isLeaf()) {
        while (i >= 0 && key < node->getKeyAt(i)) {
            i--;
        }
        node->insertKeyValueAt(i + 1, key, value); // 插入键值对

    } else {
        // 如果是非叶节点，找到合适的子节点
        while (i >= 0 && compareTo(key, node->getKeyAt(i)) < 0) {
            i--;
        }
        i++;

        // 如果子节点满了，先分裂
        if (node->getChildAt(i)->isFull()) {
            splitChild(node, i, node->getChildAt(i));

            if (key > node->getKeyAt(i)) {
                i++;
            }
        }

        insertNotFull(node->getChildAt(i), key, value); // 递归插入到子节点
    }
}

template <typename KeyType, typename ValueType>
bool BTree<KeyType, ValueType>::validateBTreeNode(TreeNode *node, int min_keys, int max_keys, int current_depth, std::set<int> &leaf_depths) {
    // 检查节点是否为空
    if (node == nullptr) {
        return true;
    }

    int key_count = node->getKeyNums();
    int child_count = node->childNums;

    // 检查键值数是否在合法范围内
    if (key_count < min_keys || key_count > max_keys) {
        std::cerr << "节点的键值数量不合法：" << key_count << "node key为" << node->keys[0] << std::endl;
        return false;
    }

    // 如果是叶节点，记录其深度
    if (node->isLeaf()) {
        leaf_depths.insert(current_depth);
        if (leaf_depths.size() > 1) {
            std::cerr << "叶节点的深度不一致：" << current_depth << std::endl;
            return false;
        }
    } else {
        // 非叶节点：检查子节点数是否等于键值数加1
        if (child_count != key_count + 1) {
            std::cerr << "非叶节点的子节点数量不合法：" << child_count << "node key为" << node->keys[0] << std::endl;
            return false;
        }

        // 检查键值是否有序
        for (int i = 1; i < key_count; i++) {
            if (node->getKeyAt(i - 1) >= node->getKeyAt(i)) {
                std::cerr << "节点的键值顺序不正确：" << node->getKeyAt(i - 1) << " >= " << node->getKeyAt(i) << std::endl;
                return false;
            }
        }

        // 递归检查子节点
        for (int i = 0; i < child_count; i++) {
            // 左子节点的所有键必须小于当前节点的键值
            if (i == 0) {
                if (!validateBTreeNode(node->getChildAt(i), min_keys, max_keys, current_depth + 1, leaf_depths)) {
                    return false;
                }
            }
            // 中间子节点
            else if (i < key_count) {
                if (node->getKeyAt(i - 1) >= node->getChildAt(i)->getKeyAt(0)) {
                    std::cerr << "子节点键值范围不正确：" << node->getKeyAt(i - 1) << " >= " << node->getChildAt(i)->getKeyAt(0) << std::endl;
                    return false;
                }
                if (!validateBTreeNode(node->getChildAt(i), min_keys, max_keys, current_depth + 1, leaf_depths)) {
                    return false;
                }
            }
            // 右子节点的所有键必须大于当前节点的键值
            else {
                if (!validateBTreeNode(node->getChildAt(i), min_keys, max_keys, current_depth + 1, leaf_depths)) {
                    return false;
                }
            }
        }
    }

    return true;
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::printTreeHelper(TreeNode *node, int depth, bool isLastChild) const {
    if (node == nullptr) {
        return;
    }

    // 打印缩进
    for (int i = 0; i < depth - 1; i++) {
        std::cout << (isLastChild ? "    " : "│   ");
    }

    if (depth > 0) {
        std::cout << (isLastChild ? "└── " : "├── ");
    }

    // 打印节点中的键值
    std::cout << "[";
    for (int i = 0; i < node->getKeyNums(); i++) {
        std::cout << node->getKeyAt(i);// << " childNum: " << node->childNums;
        if (i < node->getKeyNums() - 1) {
            std::cout << " | ";
        }
    }
    std::cout << "]" << std::endl;

    // 递归打印子节点
    if (!node->isLeaf()) {
        for (int i = 0; i < node->childNums; i++) {
            bool lastChild = (i == node->getKeyNums());
            printTreeHelper(node->getChildAt(i), depth + 1, lastChild);
        }
    }
}



#endif