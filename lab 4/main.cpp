#include "Job.h"

vector<Job> generateRandomJobVec(int time_min, int time_max, int max_time, int len) {
    vector<Job> vec;
    vec.resize(len);
    for (int i = 0; i < len; i++) {
        int profit = RandomGenerator::getRandomValue<int>(50, 300);
        int duration = RandomGenerator::getRandomValue<int>(time_min, time_max);
        int deadLine = RandomGenerator::getRandomValue<int>(duration, max_time);

        vec[i] = Job(i, duration, deadLine, profit);
    }
    return vec;
}

void printSplitline() {
    cout << "---------------------------------------------------------------------------------------------------------\n";
}

void test_1_greedy(vector<Job> jobs) {

    SolutionGreedy solution(jobs);
    auto result = solution.scheduleJob();
    std::cout << "Max Profit greedy: " << result.first << std::endl << "[ ";
    for (const auto &job : result.second) {
        cout << job.id << " ";
    }
    cout << "]\n";
}

void test_1_dp(vector<Job> jobs) {
    SolutionDP solution(jobs);
    auto result = solution.scheduleJob();
    std::cout << "Max Profit dp: " << result.first << std::endl << "[ ";
    for (const auto &job : result.second) {
        cout << job.id << " ";
    }
    cout << "]\n";
}

void test_large(int time_min, int time_max, int max_time, int len) {
    printSplitline();
    cout << "Scale:" << len << endl;
    auto jobs = generateRandomJobVec(time_min, time_max, max_time, len);
    SolutionGreedy solutionGreedy(jobs);
    SolutionDP solutionDP(jobs);
    auto start = std::chrono::high_resolution_clock::now();
    int maxProfit = solutionGreedy.scheduleJob().first;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Max profit Greedy: " << maxProfit << std::endl;
    cout << "Time Consumption of Greedy for scale " << len << " is " << duration.count() << " micro seconds\n";

    start = std::chrono::high_resolution_clock::now();
    maxProfit = solutionDP.scheduleJob().first;
    std::cout << "Max profit DP: " << maxProfit << std::endl;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Time Consumption of DP for scale " << len << " is " << duration.count() << " micro seconds\n";
    jobs.clear();
}

int main() {
    vector<Job> jobs_1 = {Job(1, 2, 4, 60), Job(2, 1, 2, 100), Job(3, 3, 6, 20), Job(4, 2, 5, 40), Job(5, 1, 3, 20), Job(6, 2, 7, 30), Job(7, 1, 4, 50), Job(8, 3, 8, 30)};

    vector<Job> jobs_2 = {Job(1, 1, 2, 20), Job(2, 2, 2, 15), Job(3, 1, 1, 25), Job(4, 2, 3, 10), Job(5, 1, 3, 5), Job(6, 3, 4, 30)};

    vector<Job> jobs_3 = {Job(1, 1, 2, 10), Job(2, 2, 4, 5), Job(3, 1, 3, 8)};

    test_1_greedy(jobs_1);
    test_1_dp(jobs_1);
    test_1_greedy(jobs_2);
    test_1_dp(jobs_2);
    test_1_greedy(jobs_3);
    test_1_dp(jobs_3);
    test_large(0, 15, 200, 500);
    test_large(0, 50, 300, 1000);
    test_large(0, 50, 1000, 2000);
    test_large(0, 50, 5000, 5000);
    test_large(0, 50, 10000, 10000);
    test_large(0, 50, 10000, 20000);
}