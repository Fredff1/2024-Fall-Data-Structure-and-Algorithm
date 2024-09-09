#ifndef SORT_H
#define SORT_H

#include "Utilility.h"
#include <chrono>

class SortSet {
  private:
    template <typename T>
    static void swap(T &first, T &second);

    template <typename T>
    static void swap(vector<T> &vec, int index_1, int index_2);

    /*
    The recursive part of mergeSort
    */
    template <typename T>
    static void mergeSortHealper(vector<T> &vec, int begin_index, int end_index);

    /*
    THe recursive part of merge-insertSort
    */
    template <typename T>
    static void mergeSortHelperMixed(vector<T> &vec, int begin_index, int end_index, int change_len);

    /*
    Merge two vectors
    */
    template <typename T>
    static void mergeVector(vector<T> &vec, int left_start, int right_start, int end_index);

  public:
    template <typename T>
    static void insertSort(vector<T> &vec);

    template <typename T>
    static void insertSort(vector<T> &vec, int begin, int end);

    template <typename T>
    static void mergeSort(vector<T> &vec);

    template <typename T>
    static void mergeSortMixed(vector<T> &vec, int change_len);

    static void mergeSortMixed(string &str, int change_len);
};

template <typename T>
void SortSet::swap(T &first, T &second) {
    auto temp = first;
    first = second;
    second = temp;
}

template <typename T>
void SortSet::swap(vector<T> &vec, int index_1, int index_2) {
    auto temp = vec[index_1];
    vec[index_1] = vec[index_2];
    vec[index_2] = temp;
}

template <typename T>
void SortSet::mergeSortHealper(vector<T> &vec, int begin_index, int end_index) {
    if (end_index <= begin_index) {
        return;
    }
    int mid_index = (begin_index + end_index) / 2;
    mergeSortHealper(vec, begin_index, mid_index);
    mergeSortHealper(vec, mid_index + 1, end_index);
    mergeVector(vec, begin_index, mid_index + 1, end_index);
}

template <typename T>
void SortSet::insertSort(vector<T> &vec, int begin, int end) {
    int len = end - begin + 1;
    for (int i = begin; i < end + 1; i++) {
        T current_value = vec[i];
        int j = i - 1;
        while (j >= begin && vec[j] > current_value) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = current_value;
    }
}

template <typename T>
void SortSet::mergeSortHelperMixed(vector<T> &vec, int begin_index, int end_index, int change_len) {
    if ((end_index - begin_index + 1) <= change_len) {
        insertSort(vec, begin_index, end_index);
    } else {
        int mid_index = (begin_index + end_index) / 2;
        mergeSortHelperMixed(vec, begin_index, mid_index, change_len);
        mergeSortHelperMixed(vec, mid_index + 1, end_index, change_len);
        mergeVector(vec, begin_index, mid_index + 1, end_index);
    }
}

template <typename T>
void SortSet::mergeVector(vector<T> &vec, int left_start, int right_start, int end_index) {
    vector<T> left_arr;
    vector<T> right_arr;

    for (int i = left_start; i < right_start; i++) {
        left_arr.push_back(vec[i]);
    }

    for (int i = right_start; i <= end_index; i++) {
        right_arr.push_back(vec[i]);
    }

    int i = 0, j = 0;
    int target_index = left_start;
    while (i < left_arr.size() && j < right_arr.size()) {
        if (left_arr[i] <= right_arr[j]) {
            vec[target_index] = left_arr[i];
            i++;
        } else {
            vec[target_index] = right_arr[j];
            j++;
        }
        target_index++;
    }

    while (i < left_arr.size()) {
        vec[target_index] = left_arr[i];
        i++;
        target_index++;
    }

    while (j < right_arr.size()) {
        vec[target_index] = right_arr[j];
        j++;
        target_index++;
    }
}

template <typename T>
void SortSet::insertSort(vector<T> &vec) {
    int len = vec.size();
    for (int i = 1; i < len; i++) {
        T current_value = vec[i];
        int j = i - 1;
        while (j >= 0 && vec[j] > current_value) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = current_value;
    }
}

template <typename T>
void SortSet::mergeSort(vector<T> &vec) {
    mergeSortHealper(vec, 0, vec.size() - 1);
}

template <typename T>
void SortSet::mergeSortMixed(vector<T> &vec, int change_len) {
    mergeSortHelperMixed(vec, 0, vec.size() - 1, change_len);
}

void SortSet::mergeSortMixed(string &str, int change_len) {
    vector<char> vec_str(str.begin(), str.end());
    SortSet::mergeSortMixed(vec_str, change_len);
    std::string new_str(vec_str.begin(), vec_str.end());
    str = new_str;
}

#endif
