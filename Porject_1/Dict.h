#ifndef DICT_H
#define DICT_H
#include "FileIO.h"
#include "TreeMap.h"
#include <memory>
#include <stdexcept>
#include <string>

enum MapType { RB_TREEMAP_TYPE, B_TREEMAP_TYPE };

class Dictionary {
    using string = std::string;

  private:
    std::unique_ptr<Map<std::string, std::string>> dataMap;

    std::unique_ptr<Map<std::string, std::string>> initMap(MapType type);

    void initialize(string file_path = "1_initial.txt");

  public:
    Dictionary(MapType type,string file_path="1_initial.txt") : dataMap(initMap(type)) {
        initialize();
    }

    string search(string english_key){
        return *(*dataMap).get(english_key);
    }

    vector<std::pair<string,string>> searchBatch(string start_english,string end_rnglish){
        int index_start=(*dataMap).indexOf(start_english);
        int index_end=(*dataMap).indexOf(end_rnglish);
        if(index_start<0||index_end<0){
            throw std::invalid_argument("Invalid input range");
        }
        vector<std::pair<string,string>> returnVector;
        for(int i=index_start;i<=index_end;i++){
            auto value=(*dataMap).findByIndex(i);
            returnVector.push_back(value);
        }
        return returnVector;
    }



    void print(std::string indent = "") const{
        (*dataMap).print(indent);
    }

    void insert(const string& key,string& value){
        (*dataMap).insert(key,value);
    }

    void erase(const string& key){
        (*dataMap).erase(key);
    }

    int size() const{
        return (*dataMap).size();
    }

    bool empty() const{
        return (*dataMap).empty();
    }
};

void Dictionary::initialize(string file_path) {
    auto init_data = FileIO::readFile(file_path);
    for (const auto &target_pair : init_data.terms) {
        auto myKey = target_pair.first;
        auto myValue = target_pair.second;
        (*dataMap).insert(myKey, myValue);
    }
}

std::unique_ptr<Map<std::string, std::string>> Dictionary::initMap(MapType type) {
        if (type == RB_TREEMAP_TYPE) {
            return std::make_unique<TreeMap<std::string, std::string>>();
        } else if (type == B_TREEMAP_TYPE) {
            return std::make_unique<TreeMap<std::string, std::string>>(); // 假设未来你会有B树的实现
        } else {
            throw std::invalid_argument("Invalid Tree type");
        }
    }

#endif
