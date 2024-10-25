# Lab 3 实验报告

## 1 用数组表示d-heap

参考二叉堆，d-heap也可以通过一个节点索引推得其父、子节点，公式为:
对于数组中index为i的节点
父节点 index_father=(i-1)/d
它的第j个子节点(j< d) index_child=d*i+j+1
或j从1开始，则忽略公式最后的+1

## 2 堆的高度 
对于每个节点，有d个子节点（除了leaf节点），因此有$h=log_d{n}$
这就是树的高度

## 3 Extract Max操作的复杂度
参考正常的堆，需要将堆尾的元素放到根节点，向下调整堆每层都需要比较d个节点找到最大的那个。至多进行h次。因此复杂度为$dlog_d{n}$

## Insert 操作的复杂度
参考正常的堆，将新元素放到堆尾，然后向上调整堆。每次比较1次，之多进行h次这样的比较。复杂度为$log_d{n}$

## Increase-key操作的复杂度
本质上，这和insert是一样的，最坏情况下，操作了叶节点，最多也需要进行h层的每层1次比较，复杂度是$log_d{n}$

## 代码实现
本次代码包括了一个Heap.h文件，其中包含了我很久之前写的Heap类以及本次课实现的D_Heap类，你可以无视前一个类（写的很烂）
D_Heap类提供以下的方法： 
validate 验证heap是否满足性质，返回bool值
insert 添加一个元素
extract 取出根节点上的值，取决于这是一个最大堆还是最小堆
changeValueAt 更改对应index上的值
getValueAt 得到对应index上的值
printHeap 打印数组，不展示堆结构，需要手动观察

D_Heap使用以下核心函数
heapIfDown 向下遍历保持堆的性质 不断遍历自己的子节点并swap直到所有子节点比自己小
heapIfUp 向上遍历保持堆的性质 不断找到index对应父节点并进行比较
std::function<int(T,T)> compareTo 在构造的时候传入的lambda函数，默认值为直接使用> 等运算符号进行比较并会构造一个最大堆
支持自定义比较方法和根据返回值决定是最大堆还是最小堆。

main 函数进行了简单的时间测算，因为是logn数量级的操作，三种操作的时间随着数量级增加变化不明显，但总体来说，
d的增加会增加heapIfDown相关操作的代价(因为全比较一遍),减少heapIfUp相关操作的代价(因为高度降低了)
实际运行时间太短，差距不是非常明显 
validate是最耗时的，因为他需要检查整个数组