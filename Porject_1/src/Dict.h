#ifndef DICT_H
#define DICT_H
#include "BtreeMap.h"
#include "FileIO.h"
#include "RBTreeMap.h"
#include <memory>
#include <stdexcept>
#include <string>

enum MapType { RB_TREEMAP_TYPE, B_TREEMAP_TYPE };

class Dictionary {
    using string = std::string;

  private:
    std::unique_ptr<Map<std::string, std::string>> dataMap;

    std::unique_ptr<Map<std::string, std::string>> initMap(MapType type);

    void initialize(string file_path);

  public:
    Dictionary(MapType type, string file_path) : dataMap(initMap(type)) {
        initialize(file_path);
    }

    Dictionary(MapType type) : dataMap(initMap(type)) {
    }

    ~Dictionary() {
    }

    void readBig(const string &path,int maxIndex=5000000) {
    
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return;
        }

        // 读取文件的第一行作为 mode

        std::string line;
        int idx=0;
        while (std::getline(file, line)&&idx<=maxIndex) {
            try{
                std::istringstream iss(line);
                std::string english, chinese;
                if (std::getline(iss, english, ' ')) {
                    std::getline(iss, chinese); // 如果有中文词，则读取其余部分为中文
                    (*dataMap).insert(english,chinese);
                }
                idx++;
                if(idx%5000000==0){
                    cout<<"Current Index :"<<idx<<endl;
                }
               
            }catch(std::exception& e){
                cout<<e.what();
            }
            
        }
        
    }

    string search(string english_key) {
        const auto &map = dataMap.get();
        string *value = (*map).get(english_key);
        if (value == nullptr) {
            throw std::invalid_argument("key dose not exist");
        }
        return *value;
    }

    std::pair<string,string> findByIndex(int index){
        std::pair<string,string> pair=(*dataMap).findByIndex(index);
        return pair;
    }

    vector<std::pair<string, string>> searchBatch(string start_english, string end_english) {
        int index_start = (*dataMap).indexOf(start_english);
        int index_end = (*dataMap).indexOf(end_english);
        if (index_start < 0 || index_end < 0) {
            throw std::invalid_argument("Invalid input range");
        }
        vector<std::pair<string, string>> returnVector;
        for (int i = index_start; i <= index_end; i++) {
            auto value = (*dataMap).findByIndex(i);
            returnVector.push_back(value);
        }
        return returnVector;
    }

    void readDocument(const std::string &path) {
        auto target = FileIO::readFile(path);
        if (target.mode == "INSERT") {
            for (int i = 0; i < target.terms.size(); i++) {
                auto pair = target.terms[i];
                auto key = pair.first;
                auto value = pair.second;
                insert(key, value);
            }
        } else if (target.mode == "DELETE") {
            for (int i = 0; i < target.terms.size(); i++) {
                auto pair = target.terms[i];
                auto key = pair.first;
                erase(key);
            }
        } else {
            readBig(path);
        }
    }

    void writeTree(const string& path){
        FileIO::writeFile(path,toString());
    }

    void switchToOtherMap(MapType type) {
        auto map_ptr = initMap(type); // 使用 initMap 函数初始化新的 Map

        for (int i = 0; i < (*dataMap).size(); i++) {
            auto pair = (*dataMap).findByIndex(i);
            (*map_ptr).insert(pair.first, pair.second);
        }

        dataMap = std::move(map_ptr); // 转移所有权给 dataMap
    }

    void print(int indent = 2) const {
        //(*dataMap).print(indent);
        cout << toString(indent);
    }

    void insert(const string &key, string &value) {
        (*dataMap).insert(key, value);
    }

    void erase(const string &key) {
        (*dataMap).erase(key);
    }

    void set(const string &key, const string &value) {
        (*dataMap).set(key, value);
    }

    int size() const {
        return (*dataMap).size();
    }

    std::string toString(int indent = 2) const {
        return (*dataMap).toString(indent);
    }

    bool empty() const {
        return (*dataMap).empty();
    }

    void clear() {
        (*dataMap).clear();
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
        return std::make_unique<RBTreeMap<std::string, std::string>>();
    } else if (type == B_TREEMAP_TYPE) {
        return std::make_unique<BTreeMap<std::string, std::string>>(); // 假设未来你会有B树的实现
    } else {
        throw std::invalid_argument("Invalid Tree type");
    }
}

#endif
