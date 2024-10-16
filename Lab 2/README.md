# 实验报告 2024/9/24

## 1 代码实现

代码首先实现了一个简单的矩阵类并重载了必要的运算符，提供了一些基本方法（基本乘法直接由重载的*号实现）
然后，代码根据书上的指导，实现了Strassen算法（包括分割矩阵 用子矩阵计算7个帮助矩阵 计算结果的四个子矩阵 合并）。略微改变了7个计算用矩阵对应的序号
同时支持了混合算法
具体实现见源代码文件Matrix.h

## 2 对两种算法的实验比较

### 2.1 实验结果（代码输出）
-----------------------------------------------------------------
Test scale: 10
Normal Algorithm took 1e-06 seconds
Merged algorithm took 0.000333 seconds
Strassen algorithm change to normal algorithm at size 2
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.001313 seconds and strassen algorithm change to normal algorithm at size 1
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 20
Normal Algorithm took 4e-06 seconds
Merged algorithm took 0.000608 seconds
Strassen algorithm change to normal algorithm at size 5
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.002796 seconds and strassen algorithm change to normal algorithm at size 2
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 64
Normal Algorithm took 0.000112 seconds
Merged algorithm took 0.000449 seconds
Strassen algorithm change to normal algorithm at size 16
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.001287 seconds and strassen algorithm change to normal algorithm at size 8
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 128
Normal Algorithm took 0.000834 seconds
Merged algorithm took 0.002447 seconds
Strassen algorithm change to normal algorithm at size 32
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.003353 seconds and strassen algorithm change to normal algorithm at size 16
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 150
Normal Algorithm took 0.001462 seconds
Merged algorithm took 0.010342 seconds
Strassen algorithm change to normal algorithm at size 37
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.020505 seconds and strassen algorithm change to normal algorithm at size 18
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 256
Normal Algorithm took 0.007138 seconds
Merged algorithm took 0.007641 seconds
Strassen algorithm change to normal algorithm at size 64
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.010049 seconds and strassen algorithm change to normal algorithm at size 32
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 512
Normal Algorithm took 0.05753 seconds
Merged algorithm took 0.049776 seconds
Strassen algorithm change to normal algorithm at size 128
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 0.055942 seconds and strassen algorithm change to normal algorithm at size 64
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 1025
Normal Algorithm took 0.556446 seconds
Merged algorithm took 2.21633 seconds
Strassen algorithm change to normal algorithm at size 256
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 2.26832 seconds and strassen algorithm change to normal algorithm at size 128
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 2048
Normal Algorithm took 9.28063 seconds
Merged algorithm took 3.09298 seconds
Strassen algorithm change to normal algorithm at size 512
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 2.22747 seconds and strassen algorithm change to normal algorithm at size 256
The results of two algorithms are same
-----------------------------------------------------------------
-----------------------------------------------------------------
Test scale: 4096
Normal Algorithm took 216.575 seconds
Merged algorithm took 42.4252 seconds
Strassen algorithm change to normal algorithm at size 1024
The results of two algorithms are same
-----------------------------------------------------------------
Merged algorithm took 29.9593 seconds and strassen algorithm change to normal algorithm at size 512
The results of two algorithms are same
-----------------------------------------------------------------

### 2.2结果表格

| Test Scale | Algorithm              | Time (seconds) | Change to Normal Algorithm at Size | Results Match |
|------------|-------------------------|----------------|-------------------------------------|---------------|
| 10         | Normal Algorithm        | 1e-06          | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.000333       | 2                                   | Yes           |
|            | Merged Algorithm 2      | 0.001313       | 1                                   | Yes           |
| 20         | Normal Algorithm        | 4e-06          | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.000608       | 5                                   | Yes           |
|            | Merged Algorithm 2      | 0.002796       | 2                                   | Yes           |
| 64         | Normal Algorithm        | 0.000112       | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.000449       | 16                                  | Yes           |
|            | Merged Algorithm 2      | 0.001287       | 8                                   | Yes           |
| 128        | Normal Algorithm        | 0.000834       | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.002447       | 32                                  | Yes           |
|            | Merged Algorithm 2      | 0.003353       | 16                                  | Yes           |
| 150        | Normal Algorithm        | 0.001462       | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.010342       | 37                                  | Yes           |
|            | Merged Algorithm 2      | 0.020505       | 18                                  | Yes           |
| 256        | Normal Algorithm        | 0.007138       | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.007641       | 64                                  | Yes           |
|            | Merged Algorithm 2      | 0.010049       | 32                                  | Yes           |
| 512        | Normal Algorithm        | 0.05753        | -                                   | Yes           |
|            | Merged Algorithm 1      | 0.049776       | 128                                 | Yes           |
|            | Merged Algorithm 2      | 0.055942       | 64                                  | Yes           |
| 1025       | Normal Algorithm        | 0.556446       | -                                   | Yes           |
|            | Merged Algorithm 1      | 2.21633        | 256                                 | Yes           |
|            | Merged Algorithm 2      | 2.26832        | 128                                 | Yes           |
| 2048       | Normal Algorithm        | 9.28063        | -                                   | Yes           |
|            | Merged Algorithm 1      | 3.09298        | 512                                 | Yes           |
|            | Merged Algorithm 2      | 2.22747        | 256                                 | Yes           |
| 4096       | Normal Algorithm        | 216.575        | -                                   | Yes           |
|            | Merged Algorithm 1      | 42.4252        | 1024                                | Yes           |
|            | Merged Algorithm 2      | 29.9593        | 512                                 | Yes           |

### 2.3 结果分析

可以看到，当维度不够大的时候，混合算法不如普通算法。当维度较大时，因为混合算法复杂度低，所以其速度相对快。
这里没有测试单独的strassen算法，因为这个算法对于维度不够大的情况效率很低，会导致上述模拟花费过多时间。其平均速率大约只有原算法的几十到几百分之一

当维度接近2的整数次幂时，混合算法的效率会相对较高，当正好超过2的整数幂一定量时，其速度会损失很多（因为需要补全）

由上述数据还可以发现：当维度在512时，普通算法依然占优，到了1024时混合算法相对更有优势。在中间区间，接近512普通更优，接近1024混合更优

## 3 结合理论

由书中推导可知，strassen算法的时间复杂度为O(n^(lg7))，低于普通算法的O(n^3)

但是在实际实现中，前者每次计算一个子矩阵都需要先分割（产生数据复制开销），再计算七个计算用矩阵和四个结果的子矩阵（数据计算、创建新矩阵开销），因此，当n不够大的时候，这些都会带来非常大的计算开销，导致其远远慢于正常计算

混合算法可以减少递归开销和小矩阵时不必要的大量复制，在n不是很大时就能得到不错的性能，拥有优势

在生产环境中，混合算法的另一个优势在于适配并行计算，但这个的代码实现过于复杂（尤其是竞争关系和数据共享等方面）。故只明确有这个作用

