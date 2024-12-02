#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QPixmap>


#include "Controller.hpp"
#include "GlobalFlags.hpp"






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
    //map.MST("A");
    //cout<<"---------------------------";
    map.MST("A","kruskal");
    // map.shortestPathFunc("A","L");
    // map.shortestPathFunc("A","L","bellmanFord");
    // map.MST("A");
    // QApplication app(argc, argv);
    // Controller controller;
    // controller.showGUI();
    // return app.exec();

}

#endif

#ifdef MAIN
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Controller controller;
    controller.showGUI();
    return app.exec();
}
#endif




