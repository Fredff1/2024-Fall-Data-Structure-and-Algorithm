#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include "Dict.h"
#include "Utilility.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

using std::string;



class CommandParser {
  public:
    CommandParser(Dictionary &dict) : dict(dict) {
    }

    void readCommand(const std::string &command);

  private:
    Dictionary &dict;
    bool initilaized = false;

    void handleInsert(const std::vector<std::string> &args);
    void handleDelete(const std::vector<std::string> &args);
    void handleSet(const std::vector<std::string> &args);
    void handleSearch(const std::vector<std::string> &args);
    void handleSearchIndex(const std::vector<std::string> &args);
    void handleSwitch(const std::vector<std::string> &args);
    void handleBatchSearch(const std::vector<std::string> &args);
    void handleReadDocument(const std::vector<std::string> &args);
    void handlePrint(const std::vector<std::string> &args);
    void handleInit(const std::vector<std::string> &args);
    void handleHelp(const std::vector<std::string> &args);
    void handleReadBig(const std::vector<std::string> &args);
    void writeToFile(const std::vector<std::string> &args);

    std::vector<std::string> parseCommand(const std::string &command);
};

vector<std::string> CommandParser::parseCommand(const std::string &command) {
    std::istringstream iss(command);
    std::string token;
    vector<std::string> tokens;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        throw std::invalid_argument("No command provided");
    }

    return tokens;
}

void CommandParser::readCommand(const std::string &command) {
    try {
        std::vector<std::string> tokens = parseCommand(command);

        auto cmdType = tokens[0];

        if (cmdType == "insert") {
            handleInsert(tokens);
        } else if (cmdType == "delete") {
            handleDelete(tokens);
        } else if (cmdType == "set") {
            handleSet(tokens);
        } else if (cmdType == "search") {
            handleSearch(tokens);
        } else if (cmdType == "switch") {
            handleSwitch(tokens);
        } else if (cmdType == "batchsearch") {
            handleBatchSearch(tokens);
        } else if (cmdType == "readocument") {
            handleReadDocument(tokens);
        } else if (cmdType == "print") {
            handlePrint(tokens);
        } else if (cmdType == "help") {
            handleHelp(tokens);
        } else if(cmdType=="searchIndex"){
            handleSearchIndex(tokens);
        }else if(cmdType=="readBig"){
            handleReadBig(tokens);
        }else if(cmdType=="writeFile"){
            writeToFile(tokens);
        }else {
            throw std::invalid_argument("Unknown command: " + cmdType);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error occured when trying to execute command: " << e.what() << std::endl;
    }
}

void CommandParser::handleInsert(const std::vector<std::string> &args) {
    if (args.size() < 3) {
        throw std::invalid_argument("Too few arguments for insert");
    }
    std::string key = args[1];
    std::string value = args[2];
    dict.insert(key, value);
    cout<<"insert key: "<<key<<" and value: "<<value<<endl;
}
void CommandParser::handleDelete(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        throw std::invalid_argument("No key provided for delete");
    }
     std::string key = args[1];
    dict.erase(key);
    cout<<"Delete key: "<<key<<endl;
}
void CommandParser::handleSet(const std::vector<std::string> &args) {
    if (args.size() < 3) {
        throw std::invalid_argument("Too few arguments for set");
    }
    std::string key = args[1];
    std::string value = args[2];
    dict.set(key, value);
    cout<<"Set key: "<<key<<" by value: "<<value<<endl;
}
void CommandParser::handleSearch(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        throw std::invalid_argument("No key provided for search");
    }
    std::string key = args[1];
    string target = dict.search(key);
    cout << "Search result for " << key << " : " << target << endl;
}

void CommandParser::handleSwitch(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        throw std::invalid_argument("No type provided");
    }
    std::string type = args[1];
    if (type == "rbTree") {
        dict.switchToOtherMap(RB_TREEMAP_TYPE);
    } else if (type == "bTree") {
        dict.switchToOtherMap(B_TREEMAP_TYPE);
    } else {
        throw std::invalid_argument("Invalid map type");
    }
}
void CommandParser::handleBatchSearch(const std::vector<std::string> &args) {
    if (args.size() < 3) {
        throw std::invalid_argument("Keys provided are not enough");
    }
    std::string begin = args[1];
    std::string end = args[2];
    auto vec=dict.searchBatch(begin, end);
    for(const auto& data:vec){
        cout<<"key: "<<data.first<<" value: "<<data.second<<endl;
    }
}
void CommandParser::handleReadDocument(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        throw std::invalid_argument("No path provided");
    }
    std::string path = args[1];
    dict.readDocument(path);
    cout<<"read document from path "<<path<<endl;
}
void CommandParser::handlePrint(const std::vector<std::string> &args) {
    int ind = 2;
    if (args.size() >= 2) {
        std::string indentStr = args[1];
        try {
            ind = std::stoi(indentStr);
        } catch (std::exception &e) {
            cout << "Invalid indent,use default indent 2\n";
        }
    }
    dict.print(ind);
}
void CommandParser::handleInit(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        throw std::invalid_argument("No path provided");
    }
    std::string path = args[1];
    dict.readDocument(path);
    cout<<"initiliazed with document "<<path<<endl;
}

void CommandParser::handleSearchIndex(const std::vector<std::string> &args){
    int index=0;
    if (args.size() >= 2) {
        std::string indentStr = args[1];
        try {
            index = std::stoi(indentStr);
        } catch (std::exception &e) {
            cout << "Invalid indent,use default indent 2\n";
        }
    }
    auto result=dict.findByIndex(index);
    cout<<"Index "<<index<<" : "<<result.first<<" : "<<result.second<<endl;
}

void CommandParser::handleReadBig(const std::vector<std::string> &args){
    if (args.size() < 2) {
        throw std::invalid_argument("No path provided");
    }
    std::string path = args[1];
    dict.readBig(path);
    cout<<"read document from path "<<path<<endl;
}

void CommandParser::writeToFile(const std::vector<std::string> &args){
    if (args.size() < 2) {
        throw std::invalid_argument("No path provided");
    }
    std::string path = args[1];
    dict.writeTree(path);
    cout<<"Write file to path "<<path<<endl;
}

void CommandParser::handleHelp(const std::vector<std::string> &args) {
    cout<<"-------------------------------------------------------------------------"<<endl;
    cout<<"Available Commands\n";
    cout<<"insert [key] [value]\n";
    cout<<"delete [key]\n";
    cout<<"set [key] [new value]\n";
    cout<<"search [key]\n";
    cout<<"batchsearch [key_start] [key_end]\n";
    cout<<"searchIndex [index]\n";
    cout<<"switch [(rbTree)/(bTree)]\n";
    cout<<"readocument [filePath]\n";
    cout<<"writeFile [filePath]\n";
    cout<<"print [indent]\n";
    cout<<"help\n";
    cout<<"-------------------------------------------------------------------------"<<endl;
    
}
#endif
