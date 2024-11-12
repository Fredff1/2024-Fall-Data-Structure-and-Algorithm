#ifndef B_TREE_H
#define B_TREE_H
#include "TreeNode.h"
#include <functional>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>

using std::string;

template <typename KeyType, typename ValueType>
class BTree {
    using TreeNode = BTreeNode<KeyType, ValueType>;

  private:
    TreeNode *root;

    int T = 5;

    std::function<int(KeyType, KeyType)> compareTo;

    void splitChild(TreeNode *parent, int i, TreeNode *fullChild);

    void insertNotFull(TreeNode *node, const KeyType key, const ValueType &value);

    void removeHelper(TreeNode *node, KeyType key);

    std::pair<KeyType, ValueType> getPredecessor(TreeNode *node);

    std::pair<KeyType, ValueType> getSuccessor(TreeNode *node);

    void mergeNodes(TreeNode *parent, int index);

    void borrowFromPrev(TreeNode *node, int index);

    void borrowFromNext(TreeNode *node, int index);

    TreeNode *createNode(bool isLeaf) {
        TreeNode *newNode = new TreeNode(isLeaf, T);
        return newNode;
    }

    void deleteAllHelper(TreeNode *node) {
        if (node == nullptr) {
            return;
        }

        // 递归删除所有子节点
        for (int i = 0; i < node->childNums; ++i) {
            if(node->children[i]!=nullptr){
                deleteAllHelper(node->children[i]);
                node->children[i] = nullptr; // 释放后置空指针
            }
            
        }

        // 删除当前节点
        delete node;
    }

    void fixUnderFlow(TreeNode *node, int index);

    void deleteFromNonLeaf(TreeNode *node, int index);

    string toStringHelper(TreeNode *node, int depth, bool isLastChild, std::ostringstream &oss) const;

    int findIndexByKeyHelper(TreeNode *node, const KeyType &key, int cumulativeIndex) const;

    std::pair<BTreeNode<KeyType, ValueType> *, int> findNodeByIndexHelper(TreeNode *node, int index) const;

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

    ~BTree() {
        deleteAll();
    }

    void deleteAll() {
        deleteAllHelper(root);
        root = nullptr; // 清空后将根节点设置为 nullptr
    }

    void insert(const KeyType key, const ValueType &value);

    TreeNode *search(TreeNode *node, KeyType key) const;

    std::pair<KeyType &, ValueType &> findNodeByIndex(int index) const;

    int findIndexByKey(const KeyType &key) const;

    // 删除键值
    void remove(KeyType key);

    TreeNode *search(KeyType key) const {
        if (root == nullptr)
            return nullptr;
        else
            return search(root, key);
    }

    void printTree(int indent = 2) const {
        if (root != nullptr) {
            cout << toStringRepresentation();
        }
    }

    string toStringRepresentation(int indent = 2) const {
        std::ostringstream oss;
        return toStringHelper(root, 0, false, oss);
    }

    bool validateBTreeNode(TreeNode *node, int min_keys, int max_keys, int current_depth, std::set<int> &leaf_depths) const;

    bool validateBTree() const;

    int getSize() const {
        if (root == nullptr) {
            return 0;
        }
        return root->getTotalSize();
    }
};

template <typename KeyType, typename ValueType>
bool BTree<KeyType, ValueType>::validateBTree() const {

    if (root == nullptr) {
        std::cerr << "B树为空" << std::endl;
        return true;
    }

    // 储存叶节点的深度
    std::set<int> leaf_depths;

    // 根节点的键值数量可以少于T - 1，但不超过2*T - 1
    return validateBTreeNode(root, 1, 2 * T - 1, 0, leaf_depths);
}

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

/* 修复子节点量<T的情况*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::fixUnderFlow(TreeNode *node, int index) {
    if (index != 0 && node->getChildAt(index - 1)->getKeyNums() >= T) {
        // 前一个兄弟的大小有多
        borrowFromPrev(node, index);
    } else if (index != node->getKeyNums() && node->getChildAt(index + 1)->getKeyNums() >= T) {
        // 后一个兄弟的大小有多
        borrowFromNext(node, index);
    } else {
        // 否则直接找一个兄弟合并
        if (index != node->getKeyNums()) {
            mergeNodes(node, index);
        } else {
            mergeNodes(node, index - 1);
        }
    }
    node->updateSizes(); // 维护subtreesize，下同
}
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::removeHelper(TreeNode *node, KeyType key) {
    int index = 0;
    while (index < node->getKeyNums() && compareTo(node->getKeyAt(index), key) < 0) {
        index++;
    }

    // case 1 找到了对应的节点
    if (index < node->getKeyNums() && compareTo(node->getKeyAt(index), key) == 0) {
        if (node->isLeaf()) {
            // 如果是叶节点，直接删除
            node->removeKeyValueAt(index);

        } else {
            // 进一步处理
            deleteFromNonLeaf(node, index);
        }
    } else {
        if (node->isLeaf()) {
            // 这个情况下节点不存在
            return;

        } else {
            // 当key大于所有的储存的key时，有可能会出现越界
            bool flag = (index == node->getKeyNums());

            // 子节点不够删，那么就先把他补充到可以删的大小
            // 如果index指向最右的儿子且fix的时候发生了合并，合并函数会合并[-2:-1]的节点
            // 因此需要特殊考虑防止越界
            if (node->getChildAt(index)->getKeyNums() < T) {
                fixUnderFlow(node, index);
            }

            // 可能会发生合并，这个时候，child会减少，因而当index本来是最后一个的时候，需要访问前一个儿子防止越界
            if (flag && index > node->getKeyNums()) {
                removeHelper(node->getChildAt(index - 1), key);
            } else {
                removeHelper(node->getChildAt(index), key);
            }
        }
    }
    node->updateSizes();
}

template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::deleteFromNonLeaf(TreeNode *node, int index) {
    KeyType key = node->getKeyAt(index);

    // case 1 左子树够大，找前驱节点替换待删除节点，再删除前驱节点
    if (node->getChildAt(index)->getKeyNums() >= T) {
        std::pair<KeyType, ValueType> pred = getPredecessor(node->getChildAt(index));
        node->setKeyValueAt(index, pred.first, pred.second);
        removeHelper(node->getChildAt(index), pred.first);
    }
    // case 2 右子树足够大，找后继节点
    else if (node->getChildAt(index + 1)->getKeyNums() >= T) {
        std::pair<KeyType, ValueType> succ = getSuccessor(node->getChildAt(index + 1));
        node->setKeyValueAt(index, succ.first, succ.second);
        removeHelper(node->getChildAt(index + 1), succ.first);
    }
    // case 3 合并节点
    else {
        mergeNodes(node, index);
        removeHelper(node->getChildAt(index), key);
    }
}

/*合并node中index和Index+1的child*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::mergeNodes(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);       // left child 合并sib
    TreeNode *sibling = node->getChildAt(index + 1); // right child 被合并

    // 将 parent 的键值下移到 child，parent->keys[index] 被删除
    child->insertKeyValueAt(child->getKeyNums(), node->getKeyAt(index), node->getValueAt(index));

    // 将 sibling 的键值和子节点合并到 child 中
    for (int i = 0; i < sibling->getKeyNums(); i++) {
        child->insertKeyValueAt(child->getKeyNums(), sibling->getKeyAt(i), sibling->getValueAt(i));
    }

    if (!sibling->isLeaf()) {
        for (int i = 0; i < sibling->getChildNums(); i++) {
            child->insertChildAt(child->getChildNums(), sibling->getChildAt(i));
        }
    }

    // 将 parent 的键值左移，删除 parent->keys[index]
    node->removeKeyValueAt(index);

    // 删除 sibling 子节点，完成一次合并
    node->removeChildAt(index + 1);

    delete sibling;

    child->updateSizes();
    node->updateSizes();
}

/**从左兄弟借一个节点
parameters : index:借节点child在node的索引
*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::borrowFromPrev(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);       // 当前需要借键的子节点
    TreeNode *sibling = node->getChildAt(index - 1); // 左兄弟节点

    // 将 parent 的键值下移到 child 第一个位置，这个位置随后会被替代
    child->insertKeyValueAt(0, node->getKeyAt(index - 1), node->getValueAt(index - 1));

    // 把左兄弟最大的键值给parent对应的位置
    node->setKeyValueAt(index - 1, sibling->getKeyAt(sibling->getKeyNums() - 1), sibling->getValueAt(sibling->getKeyNums() - 1));

    // 如果左兄弟节点不是叶节点，则移动其最后一个子节点到 child
    if (!sibling->isLeaf()) {
        child->insertChildAt(0, sibling->getChildAt(sibling->getChildNums() - 1));
        sibling->removeChildAt(sibling->getChildNums() - 1); // 删除左兄弟的最后一个子节点
    }

    // 删除左兄弟的最后一个键值
    sibling->removeKeyValueAt(sibling->getKeyNums() - 1);
    child->updateSizes();
    sibling->updateSizes();
    node->updateSizes();
}

/*从右兄弟借一个节点*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::borrowFromNext(TreeNode *node, int index) {
    TreeNode *child = node->getChildAt(index);       // 当前需要借键的子节点
    TreeNode *sibling = node->getChildAt(index + 1); // 右兄弟节点

    // 同样从父亲拿对应位置的值
    child->insertKeyValueAt(child->getKeyNums(), node->getKeyAt(index), node->getValueAt(index));

    // 这次拿最小的
    node->setKeyValueAt(index, sibling->getKeyAt(0), sibling->getValueAt(0));

    if (!sibling->isLeaf()) {
        child->insertChildAt(child->getChildNums(), sibling->getChildAt(0));
        sibling->removeChildAt(0);
    }

    sibling->removeKeyValueAt(0);
    child->updateSizes();
    sibling->updateSizes();
    node->updateSizes();
}

/*找到key所在的节点*/
template <typename KeyType, typename ValueType>
BTreeNode<KeyType, ValueType> *BTree<KeyType, ValueType>::search(TreeNode *node, KeyType key) const {
    int i = 0;

    // 找到中间位置
    while (i < node->getKeyNums() && compareTo(node->getKeyAt(i), key) < 0) {
        i++;
    }

    // 相等，返回当前节点
    if (i < node->getKeyNums() && compareTo(node->getKeyAt(i), key) == 0) {
        return node;
    }

    if (node->isLeaf()) {
        return nullptr;
    }

    return search(node->getChildAt(i), key);
}

/*默认输入的node是左子树的根*/
template <typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> BTree<KeyType, ValueType>::getPredecessor(TreeNode *node) {
    // 移动到左子树中的最右节点（最大值）
    TreeNode *current = node;
    while (!current->isLeaf()) {
        current = current->getChildAt(current->getKeyNums()); // 最右子节点
    }
    return {current->getKeyAt(current->getKeyNums() - 1), current->getValueAt(current->getKeyNums() - 1)}; // 返回最后一个键
}

/*默认输入的nodee是右子树的根*/
template <typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> BTree<KeyType, ValueType>::getSuccessor(TreeNode *node) {
    TreeNode *current = node;
    while (!current->isLeaf()) {
        current = current->getChildAt(0);
    }
    return {current->getKeyAt(0), current->getValueAt(0)};
}

/*拆分一个满了的child用于插入*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::splitChild(TreeNode *parent, int i, TreeNode *fullChild) {

    // 右半部分
    TreeNode *newChild = createNode(fullChild->isLeaf());

    // Step I 将 fullChild 的T右侧键值对移动到 newChild
    for (int j = 0; j < T - 1; j++) {
        newChild->insertKeyValueAt(j, fullChild->getKeyAt(j + T), fullChild->getValueAt(j + T));
    }

    // Step II 后 T 个子节点移动到 newChild
    if (!fullChild->isLeaf()) {
        for (int j = 0; j < T; j++) {
            newChild->insertChildAt(j, fullChild->getChildAt(j + T));
        }
    }

    // Step III 将 fullChild 的中间键值提升到 parent 中
    parent->insertKeyValueAt(i, fullChild->getKeyAt(T - 1), fullChild->getValueAt(T - 1));

    // 更新 fullChild 中键值数量和儿子数量，相当于调用T次remove函数
    fullChild->keyNums = T - 1;
    if (!fullChild->isLeaf()) {
        fullChild->childNums = T;
    }

    // 将 newChild 插入到 parent 的 i+1 位置
    parent->insertChildAt(i + 1, newChild);

    fullChild->updateSizes();
    newChild->updateSizes();
    parent->updateSizes();
}

/*插入一个键值对，重复会替代原有的值*/
template <typename KeyType, typename ValueType>
void BTree<KeyType, ValueType>::insert(const KeyType key, const ValueType &value) {
    TreeNode *nodeExist = search(key);
    if (nodeExist != nullptr) {
        cout << "The key " << key << " already exsists!" << endl;
        for (int i = 0; i < nodeExist->getKeyNums(); i++) {
            if (compareTo(nodeExist->getKeyAt(i), key) == 0) {
                nodeExist->setValueAt(i, value);
            }
        }
        return;
    }

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
void BTree<KeyType, ValueType>::insertNotFull(TreeNode *node, const KeyType key, const ValueType &value) {

    int i = node->getKeyNums() - 1;

    if (node->isLeaf()) {
        while (i >= 0 && key < node->getKeyAt(i)) {
            i--;
        }
        node->insertKeyValueAt(i + 1, key, value); // 叶节点直接插入

    } else {
        // 如果是非叶节点，找到合适的子节点
        while (i >= 0 && compareTo(key, node->getKeyAt(i)) < 0) {
            i--;
        }
        i++;

        // 如果子节点满了，先分裂再插入
        if (node->getChildAt(i)->isFull()) {
            splitChild(node, i, node->getChildAt(i));

            if (key > node->getKeyAt(i)) {
                i++;
            }
        }

        insertNotFull(node->getChildAt(i), key, value); 
    }
    node->updateSizes(); 
}

template <typename KeyType, typename ValueType>
bool BTree<KeyType, ValueType>::validateBTreeNode(TreeNode *node, int min_keys, int max_keys, int current_depth, std::set<int> &leaf_depths) const {

    if (node == nullptr) {
        return true;
    }

    int key_count = node->getKeyNums();
    int child_count = node->childNums;

    if (key_count < min_keys || key_count > max_keys) {
        std::cerr << "节点的键值数量不合法：" << key_count << "node key为" << node->keys[0] << std::endl;
        return false;
    }


    if (node->isLeaf()) {
        leaf_depths.insert(current_depth);
        if (leaf_depths.size() > 1) {
            std::cerr << "叶节点的深度不一致：" << current_depth << std::endl;
            return false;
        }
    } else {

        if (child_count != key_count + 1) {
            std::cerr << "非叶节点的子节点数量不合法：" << child_count << "node key为" << node->keys[0] << std::endl;
            return false;
        }


        for (int i = 1; i < key_count; i++) {
            if (node->getKeyAt(i - 1) >= node->getKeyAt(i)) {
                std::cerr << "节点的键值顺序不正确：" << node->getKeyAt(i - 1) << " >= " << node->getKeyAt(i) << std::endl;
                return false;
            }
        }

       
        for (int i = 0; i < child_count; i++) {

            if (i == 0) {
                if (!validateBTreeNode(node->getChildAt(i), min_keys, max_keys, current_depth + 1, leaf_depths)) {
                    return false;
                }
            }
           
            else if (i < key_count) {
                if (node->getKeyAt(i - 1) >= node->getChildAt(i)->getKeyAt(0)) {
                    std::cerr << "子节点键值范围不正确：" << node->getKeyAt(i - 1) << " >= " << node->getChildAt(i)->getKeyAt(0) << std::endl;
                    return false;
                }
                if (!validateBTreeNode(node->getChildAt(i), min_keys, max_keys, current_depth + 1, leaf_depths)) {
                    return false;
                }
            }
           
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
std::pair<KeyType &, ValueType &> BTree<KeyType, ValueType>::findNodeByIndex(int index) const {
    if (root == nullptr) {
        throw std::runtime_error("The tree is empty!");
    }


    if (index < 0 || index >= root->getTotalSize()) {
        throw std::out_of_range("Index out of range");
    }
    
    std::pair<BTreeNode<KeyType, ValueType> *, int> node_pair = findNodeByIndexHelper(root, index);

    TreeNode *node = node_pair.first;
    auto targetIdx = node_pair.second;
    return {node->getKeyAt(targetIdx), node->getValueAt(targetIdx)};
}

template <typename KeyType, typename ValueType>
std::pair<BTreeNode<KeyType, ValueType> *, int> BTree<KeyType, ValueType>::findNodeByIndexHelper(TreeNode *node, int index) const {
    if (node->isLeaf()) {
        if (index < 0 || index >= node->getKeyNums()) {
            throw std::out_of_range("Error occured when searching");
        }
        return {node, index}; 
    } else {
        int currentIndex = 0;

        
        for (int i = 0; i < node->getKeyNums(); i++) {
            int leftTreeSize = node->getChildAt(i) ? node->getChildAt(i)->getTotalSize() : 0;

            
            if (index < currentIndex + leftTreeSize) {
                return findNodeByIndexHelper(node->getChildAt(i), index - currentIndex);
            }

            
            currentIndex += leftTreeSize;
            if (index == currentIndex) {
                return {node, i}; 
            }


            currentIndex++;
        }


        return findNodeByIndexHelper(node->getChildAt(node->getKeyNums()), index - currentIndex);
    }
}

template <typename KeyType, typename ValueType>
int BTree<KeyType, ValueType>::findIndexByKey(const KeyType &key) const {
    if (root == nullptr) {
        throw std::runtime_error("Tree is empty!");
    }

    return findIndexByKeyHelper(root, key, 0);
}

template <typename KeyType, typename ValueType>
int BTree<KeyType, ValueType>::findIndexByKeyHelper(TreeNode *node, const KeyType &key, int cumulativeIndex) const {

    int leftSize = 0;

    for (int i = 0; i < node->getKeyNums(); i++) {

        if (!node->isLeaf()) {
            leftSize = node->getChildAt(i) ? node->getChildAt(i)->getTotalSize() : 0;
        }


        if (key < node->getKeyAt(i)) {
            if (!node->isLeaf()) {
                return findIndexByKeyHelper(node->getChildAt(i), key, cumulativeIndex);
            } else {
                throw std::out_of_range("Key not found");
            }
        }


        if (key == node->getKeyAt(i)) {
            return cumulativeIndex + leftSize; 
        }

     
        cumulativeIndex += leftSize + 1; 


        if (!node->isLeaf() && i == node->getKeyNums() - 1) {
            return findIndexByKeyHelper(node->getChildAt(i + 1), key, cumulativeIndex);
        }
    }

    throw std::out_of_range("Key not found");
}

template <typename KeyType, typename ValueType>
string BTree<KeyType, ValueType>::toStringHelper(TreeNode *node, int depth, bool isLastChild, std::ostringstream &oss) const {
    if (node == nullptr) {
        return "";
    }


    for (int i = 0; i < depth - 1; i++) {
        oss << (isLastChild ? "    " : "|   ");
    }

    if (depth > 0) {
        oss << (isLastChild ? "|___ " : "|--- ");
    }


    oss << "[";
    for (int i = 0; i < node->getKeyNums(); i++) {
        oss << node->getKeyAt(i)<<": "<<node->getValueAt(i); // << " childNum: " << node->childNums;
        if (i < node->getKeyNums() - 1) {
            oss << " | ";
        }
    }
    oss << "]" << std::endl;


    if (!node->isLeaf()) {
        for (int i = 0; i < node->childNums; i++) {
            bool lastChild = (i == node->getKeyNums());
            toStringHelper(node->getChildAt(i), depth + 1, lastChild, oss);
        }
    }
    return oss.str();
}

#endif