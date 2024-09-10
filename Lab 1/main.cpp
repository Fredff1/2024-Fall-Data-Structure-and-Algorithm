#include "Sort.h"
#include <fstream>

/*
Print a vector,simply
*/
template <typename T>
void printVector(vector<T> vec, string analysis = " ") {
    cout << analysis << endl;
    for (auto &value : vec) {
        cout << value << ' ';
    }
    cout << endl;
}

/*
Validate the sort results
*/
template <typename T>
static bool is_vector_sorted(vector<T> &vec) {
    int len = vec.size();
    for (int i = 0; i < len - 1; i++) {
        if (vec[i] > vec[i + 1]) {
            cout << "Invalid" << endl << endl;
            return false;
        }
    }
    cout << "Valid" << endl << endl;
    return true;
}



void testEfficiency(int len, vector<double> &timeVec,vector<int> &indexes) {
    vector<int> arr_test = RandomGenerator::getRandomVector<int>(len, 0, 1000);
    int max_i=len/5;
    for (int i = 1; i <= max_i ; i+=(i>500?std::max((max_i/150),1):5)) {
        double time_sum = 0, repeat_time = 3;
        for (int j = 0; j < repeat_time; j++) {
            auto new_vec = arr_test;

            auto start = std::chrono::high_resolution_clock::now();

            SortSet::mergeSortMixed(new_vec, i);

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;
            time_sum += duration.count();

            // std::cout << "Time taken by function: " << duration.count() << "
            // seconds when i = "<<i << std::endl;
        }
        indexes.push_back(i);
        timeVec.push_back(time_sum / repeat_time);
    }
    // printVector<double>(time);
    double min_time = 1000;
    int index = 0;
    for (int i = 0; i < timeVec.size(); i++) {
        if (timeVec[i] <= min_time) {
            min_time = timeVec[i];
            index = i;
        }
    }

    cout << index << " is the min time" << endl;
}

void testOne(){
    vector<int> arr_test = RandomGenerator::getRandomVector<int>(100000, 0, 1000);
    auto start = std::chrono::high_resolution_clock::now();

    SortSet::mergeSortMixed(arr_test, 50);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    cout<<duration.count()<<"s"<<endl;
    
}

void testAll() {
    vector<int> target_sizes = {500, 1000, 2000, 5000, 10000, 50000,100000};
    vector<vector<double>> timeVec;
    vector<vector<int>> indexOfTime;

    std::ofstream outfile("output_mixed.csv");
    outfile << "N,i,time\n";

    for (int i = 0; i < target_sizes.size(); i++) {
        cout << "N is " << target_sizes[i] << endl;
        vector<double> new_timeVec;
        vector<int> new_indexVec;
        timeVec.push_back(new_timeVec);
        indexOfTime.push_back(new_indexVec);
        testEfficiency(target_sizes[i], timeVec[i],indexOfTime[i]);

        for (int j = 0; j < timeVec[i].size(); j++) {
            outfile << target_sizes[i] << "," << indexOfTime[i][j]<< "," << timeVec[i][j] << "\n";
        }
    }

    outfile.close();
}

int main() {
    //Test insert sort
    vector<int> array_1 = RandomGenerator::getRandomVector<int>(100, 0, 200);
    printVector<int>(array_1, "The vector before Insert Sort:");
    is_vector_sorted(array_1);
    SortSet::insertSort(array_1);
    printVector<int>(array_1, "The vector after Insert Sort:");
    is_vector_sorted(array_1);

    //Test merge sort
    vector<int> array_2 = RandomGenerator::getRandomVector<int>(100, 0, 200);
    printVector<int>(array_2, "The vector before Merge Sort:");
    is_vector_sorted(array_2);
    SortSet::mergeSort<int>(array_2);
    printVector<int>(array_2, "The vector after Merge Sort:");
    is_vector_sorted(array_2);

    //test mixed sort
    vector<int> array_3 = RandomGenerator::getRandomVector<int>(100, 0, 200);
    printVector<int>(array_3, "The vector before MergeMixed Sort:");
    is_vector_sorted(array_3);
    SortSet::mergeSortMixed<int>(array_3, 5);
    printVector<int>(array_3, "The vector after MergeMixed Sort:");
    is_vector_sorted(array_3);

    //test float 
    vector<float> array_4 = RandomGenerator::getRandomVector<float>(100, 0, 200);
    printVector<float>(array_4, "The vector before MergeMixed Sort:");
    is_vector_sorted(array_4);
    SortSet::mergeSortMixed<float>(array_4, 5);
    printVector<float>(array_4, "The vector after MergeMixed Sort:");
    is_vector_sorted(array_4);

    //test string
    string str_1 = RandomGenerator::getRandomString(100, 'a', 'z');
    cout << str_1 << endl;
    SortSet::mergeSortMixed(str_1, 10);
    cout << str_1 << endl;

    string str_2 = RandomGenerator::getRandomString(100, 'a', 'z');
    cout << str_2 << endl;
    SortSet::insertSort(str_2);
    cout << str_2 << endl;

    string str_3 = RandomGenerator::getRandomString(100, 'a', 'z');
    cout << str_3 << endl;
    SortSet::mergeSort(str_3);
    cout << str_3 << endl;

    testOne();

    //test the time
    testAll();
}
