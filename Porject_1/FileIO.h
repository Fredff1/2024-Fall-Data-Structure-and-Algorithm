#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct DataStruct {
    std::string mode;                                       // To store the mode
    std::vector<std::pair<std::string, std::string>> terms; // Vector to store pairs of English and Chinese terms
};

class FileIO {
  public:
    static DataStruct readFile(const std::string &filename) {
        DataStruct data;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return data;
        }

        // 读取文件的第一行作为 mode
        std::getline(file, data.mode);

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string english, chinese;
             if (std::getline(iss, english, ' ')) {
            std::getline(iss, chinese);  // 如果有中文词，则读取其余部分为中文
            data.terms.emplace_back(english, chinese);
            }
        }
        file.close();
        return data;
    }

    
    static void printData(const DataStruct &data) {
        std::cout << "Mode: " << data.mode << std::endl;
        for (const auto &term : data.terms) {
            std::cout << term.first << " -> " << term.second << std::endl;
        }
    }

    static void writeFile(const std::string &filename, const DataStruct &data) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "failed to write to file : " << filename << std::endl;
            return;
        }

        // 首先写入 mode
        file << data.mode << std::endl;

        // 写入所有的英文-中文词汇对
        for (const auto &term : data.terms) {
            file << term.first << " " << term.second << std::endl;
        }

        file.close();
    }
};

#endif
