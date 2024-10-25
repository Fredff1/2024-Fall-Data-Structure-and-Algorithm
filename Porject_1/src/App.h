#ifndef APP_H
#define APP_H

#include "CommandParser.h"
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <sstream>
#include <string.h>
#include <windows.h>

class DictApp {
  private:
    Dictionary dict = Dictionary(RB_TREEMAP_TYPE);
    CommandParser parser = CommandParser(dict);
    bool isRunning = true;

  public:
    DictApp() {
        SetConsoleOutputCP(936);
        SetConsoleCP(936);
    }

    ~DictApp() {
        dict.~Dictionary();
    }

    void start() {

        while (isRunning) {
            std::wcout << L"Enter command: ";

            std::string line;
            std::getline(std::cin, line);

            if (line == "exit") {
                isRunning = false;
                std::wcout << "Exiting program..." << std::endl;
            } else if (line == "shift") {
                toggleConsoleEncoding();
            } else if (line=="clear") {
                std::cout << "\033[2J\033[H";
            } else {
                parser.readCommand(line); // 将宽字符传递给解析器
            }
        }
    }
};

#endif
