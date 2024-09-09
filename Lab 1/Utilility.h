#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

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

#endif
