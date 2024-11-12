#ifndef JOB_H
#define JOB_H

#include "Utilility.h"
#include <algorithm>
#include <unordered_map>
#include <utility>

using std::pair;
using std::unordered_map;

struct Job {
    int id = 0;
    int processingTime;
    int deadLine;
    int profit;
    Job() {
    }
    Job(int id, int processingTime, int deadLine, int profit) : id(id), processingTime(processingTime), deadLine(deadLine), profit(profit) {
    }

    bool operator>(const Job &other) {
        return ((float)this->profit) / this->processingTime > ((float)other.profit) / other.processingTime;
    }

    bool operator<(const Job &other) {
        return ((float)this->profit) / this->processingTime < ((float)other.profit) / other.processingTime;
    }

    void setId(int id) {
        this->id = id;
    }
};

class Solution {
  public:
    virtual pair<int, vector<Job>> scheduleJob() = 0;
};

class SolutionGreedy : Solution {
  private:
    vector<Job> jobs;
    int maxDeadline = INT_MIN;

  public:
    SolutionGreedy(const vector<Job> &targetJobs) : jobs(targetJobs) {
        std::sort(jobs.begin(), jobs.end(), std::greater<>());
        for (const auto &job : jobs) {
            if (job.deadLine > maxDeadline) {
                maxDeadline = job.deadLine;
            }
        }
    }

    pair<int, vector<Job>> scheduleJob() {
        vector<bool> timeSLot;
        vector<pair<Job, int>> chosenJobs;
        timeSLot.resize(maxDeadline + 2, false);
        int totalProfit = 0;
        for (const auto &job : jobs) {
            int ddl = job.deadLine;
            int duration = job.processingTime;
            if (duration > ddl) {
                continue;
            }

            for (int start = ddl; start >= duration; start--) {
                if (start - duration < 1) {
                    continue;
                }
                bool isAvail = true;
                for (int i = start; i > start - duration; i--) {
                    if (timeSLot[i] == true) {
                        isAvail = false;
                        break;
                    }
                }
                if (isAvail) {
                    for (int i = start; i > start - duration; i--) {
                        timeSLot[i] = true;
                    }
                    totalProfit += job.profit;
                    chosenJobs.push_back({job, start - duration + 1});
                    break;
                }
            }
        }
        sort(chosenJobs.begin(), chosenJobs.end(), [](const pair<Job, int> &a, const pair<Job, int> &b) { return a.second < b.second; });
        vector<Job> finalTasks;
        for (const auto &job : chosenJobs) {
            finalTasks.push_back(job.first);
        }
        return {totalProfit, finalTasks};
    }
};

class SolutionDP : Solution {
  private:
    vector<Job> jobs;
    int maxDeadline = 0;

  public:
    SolutionDP(const vector<Job> &targetJobs) : jobs(targetJobs) {
        for (const auto &job : jobs) {
            if (job.deadLine > maxDeadline) {
                maxDeadline = job.deadLine;
            }
        }
    }

    /*
    使用二维动态规划数组 dp[i][t] 来表示前 i 个作业在时间 t 内能够获得的最大利润。
    dp[i][t] 表示前 i 个作业在时间 t 内的最大利润
    dp[i][t]=max(dp[i−1][t], dp[i−1][t−job[i].processingTime]+job[i].profit)
    */
    pair<int, vector<Job>> scheduleJob() {
        int n = jobs.size();
        sort(jobs.begin(), jobs.end(), [](const Job &a, const Job &b) { return a.deadLine < b.deadLine; });

        // choice[i][t] 表示在 dp[i][t] 时是否选择了第 i 个作业
        vector<vector<int>> dp(n + 1, vector<int>(maxDeadline + 1, 0));
        vector<vector<bool>> choice(n + 1, vector<bool>(maxDeadline + 1, false));

        for (int i = 1; i <= n; i++) {
            const Job &job = jobs[i - 1];
            for (int t = 0; t <= maxDeadline; t++) {
                if (t < job.processingTime || t > job.deadLine) {
                    dp[i][t] = dp[i - 1][t];
                } else {
                    int profitWithoutJob = dp[i - 1][t];
                    int profitWithJob = dp[i - 1][t - job.processingTime] + job.profit;
                    if (profitWithJob > profitWithoutJob) {
                        dp[i][t] = profitWithJob;
                        choice[i][t] = true;
                    } else {
                        dp[i][t] = profitWithoutJob;
                    }
                }
            }
        }

        int maxProfit = 0;
        int endTime = 0;
        for (int t = 0; t <= maxDeadline; t++) {
            if (dp[n][t] > maxProfit) {
                maxProfit = dp[n][t];
                endTime = t;
            }
        }

        vector<Job> selectedJobs;
        int t = endTime;
        for (int i = n; i >= 1; i--) {
            if (choice[i][t]) {
                const Job &job = jobs[i - 1];
                selectedJobs.push_back(job);
                t -= job.processingTime;
            }
        }

        reverse(selectedJobs.begin(), selectedJobs.end());

        return {maxProfit, selectedJobs};
    }
};

#endif
