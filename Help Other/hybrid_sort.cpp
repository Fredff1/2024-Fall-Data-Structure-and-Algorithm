#include <iostream>
#include <chrono>
#include <random>  // 用于生成随机数
using namespace std;
using namespace std::chrono;

/* 插入排序 */
template <typename T>
bool shouldSwap(T a, T b) {
    return a < b;
}

template <typename T>
void insertionSort(T list[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        for (int j = i - 1; j >= left; j--) {
            if (shouldSwap(list[j + 1], list[j])) {
                swap(list[j + 1], list[j]);
            } else {
                break;
            }
        }
    }
}

/* 归并排序 */
template <class T>
void merge(T list[], int left, int right, int mid) {
    T temp[right - left + 1];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (list[i] < list[j]) {
            temp[k] = list[i];
            i++;
        } else {
            temp[k] = list[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        temp[k] = list[i];
        i++;
        k++;
    }
    while (j <= right) {
        temp[k] = list[j];
        j++;
        k++;
    }
    for (int i = left; i <= right; i++) {
        list[i] = temp[i - left];
    }
}

template <typename T>
void mergeSort(T list[], int left, int right) {
    if (left >= right) return;
    else {
        int mid = (left + right) / 2;
        mergeSort(list, left, mid);
        mergeSort(list, mid + 1, right);
        merge(list, left, right, mid);
    }
}

/* 混合排序 */
template <typename T>
void hybridSort(T list[], int left, int right, int k) {
    if (right - left + 1 <= k)
        insertionSort(list, left, right);
    else {
        int mid = (left + right) / 2;
        hybridSort(list, left, mid, k);
        hybridSort(list, mid + 1, right, k);
        merge(list, left, right, mid);
    }
}

/* 生成随机数组 */
void generateRandomArray(int arr[], int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000); // 生成 1 到 10000 之间的随机数
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
}

int main() {
    int dataSize = 50000; // 测试数组的大小
    int numTests = 100;     //同一个k生成20个数组，最后取平均数
    int arr[dataSize];
    int testArr[dataSize];

    // 测试不同k值所需时间
    for (int k = 2; k <= 200; k += 2) {
        long long totalDuration = 0;
        
        for (int t = 0; t < numTests; t++) {
            // 为每次测试生成新的随机数组
            generateRandomArray(arr, dataSize);

            auto start = high_resolution_clock::now();

            hybridSort(arr, 0, dataSize - 1, k);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            
            totalDuration += duration.count();
        }
        
        // 输出当前阈值的平均排序时间
        cout << "k: " << k << ", Average time: " << totalDuration / numTests << " ns" << endl;
    }
    
    return 0;
}
