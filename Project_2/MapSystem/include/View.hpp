#ifndef VIEW_HPP
#define VIEW_HPP
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <string>


#include "Model.hpp"



class View : public QMainWindow {
  Q_OBJECT
  private:
    int height;
    int width;
    Model model;
    //using dynamic_cast to change type
    std::unordered_map<string,QObject*> qObjects;


  public:
    explicit View(Model &model, QWidget *parent = nullptr,int width=1500,int height=750);
    ~View();

    QObject* getQObject(string name){
      return qObjects.at(name);
    }

    void setPathLabelContent(string content){
      QLabel* pathLabel=dynamic_cast<QLabel*>(qObjects["pathLabel"]);
      pathLabel->setText(QString::fromStdString(content));
    }

  private slots:
    void onButtonClicked(); 
};

#endif /* F0C98FE3_6EA2_41DD_87A2_BCF91456C0BE */
