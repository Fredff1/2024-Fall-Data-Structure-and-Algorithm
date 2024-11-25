#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QPixmap>

#include "DigitalMap.hpp"


//#define DEBUG_QT
#define DEBUG_GRAPH

#define TEST

// 定义一个简单的测试函数
void printMessage(const QString &message) {
    qDebug() << "[Test Message]:" << message;
}

#ifdef DEBUG_QT

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLabel *label = new QLabel("Hello, Qt with CMake!");
    label->resize(400, 200);
    label->show();
    

    return app.exec();;  // 确保事件循环保持运行
}

#endif

#ifdef DEBUG_GRAPH



int main(int argc, char *argv[]) {
    DigitalMap map=DigitalMap();
    map.shortestPathFunc("A","F");
   
   
}


#endif


