#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <vector>
#include <chrono>
#include <functional>
#include <tuple>
#include <type_traits>
#include <windows.h>


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/*

*/

class RandomGenerator {
  public:
    template <typename T>
    static T getRandomValue(T begin, T end) {
        static std::random_device dev;
        std::mt19937 gen(dev());

        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<> dis(static_cast<int>(begin), static_cast<int>(end));
            return static_cast<T>(dis(gen));
        } else if (std::is_floating_point<T>::value) {
            std::uniform_real_distribution<> dis(static_cast<int>(begin), static_cast<int>(end));
            return static_cast<T>(dis(gen));
        } else if (std::is_same<T, char>::value) {
            std::uniform_int_distribution<> dis(static_cast<int>(begin), static_cast<int>(end));
            return static_cast<char>(dis(gen));
        }
    }

    template <typename T>
    static vector<T> getRandomVector(int len, T begin, T end) {
        vector<T> vec;
        for (int i = 0; i < len; i++) {
            vec.push_back(getRandomValue(begin, end));
        }
        return vec;
    }

    static string getRandomString(int len, char begin, char end) {
        string str;
        for (int i = 0; i < len; i++) {
            str += getRandomValue<char>(begin, end);
        }
        return str;
    }
};

template <typename Func, typename Obj, typename... Args>
auto measure_time(Func func, Obj&& obj, Args&&... args) -> std::conditional_t<std::is_void_v<decltype(std::invoke(func, std::forward<Obj>(obj), std::forward<Args>(args)...))>, std::tuple<double>, std::tuple<decltype(std::invoke(func, std::forward<Obj>(obj), std::forward<Args>(args)...)), double>> {
    auto start = std::chrono::high_resolution_clock::now();

    // 如果返回类型是 void
    if constexpr (std::is_void_v<decltype(std::invoke(func, std::forward<Obj>(obj), std::forward<Args>(args)...))>) {
        std::invoke(func, std::forward<Obj>(obj), std::forward<Args>(args)...);  // 调用成员函数或普通函数
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        return std::make_tuple(duration.count());  // 返回执行时间
    } else {
        // 如果有返回值
        auto result = std::invoke(func, std::forward<Obj>(obj), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        return std::make_tuple(result, duration.count());  // 返回结果和执行时间
    }
}

void toggleConsoleEncoding() {
    UINT inputCodePage = GetConsoleCP();
    UINT outputCodePage = GetConsoleOutputCP();

    if (inputCodePage == 65001 && outputCodePage == 65001) {
        // 当前为 UTF-8，切换到 GBK
        SetConsoleOutputCP(936);
        SetConsoleCP(936);
        std::cout << "Switched console encoding to GBK (936)." << std::endl;
    } else if (inputCodePage == 936 && outputCodePage == 936) {
        // 当前为 GBK，切换到 UTF-8
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        std::cout << "Switched console encoding to UTF-8 (65001)." << std::endl;
    } else {
        std::cout << "Console encoding is neither UTF-8 nor GBK. No changes made." << std::endl;
    }
}

#endif
