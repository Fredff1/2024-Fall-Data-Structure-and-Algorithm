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
#include <QMessageBox>
#include <string>


#include "Model.hpp"

class ConfigPage : public QWidget {
    Q_OBJECT
private:
  QPushButton *backButton;
  QVBoxLayout *layout;
  QHBoxLayout *mainLayout;

public:
    ConfigPage(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Config Page");
        setFixedSize(400, 300);
        mainLayout=new QHBoxLayout(this);
        // 创建返回按钮
        backButton = new QPushButton("Back", this);
        backButton->setFixedSize(120,60);
        backButton->setStyleSheet("QPushButton {"
                                "   background-color: #1abc9c;"
                                "   color: white;"
                                "   font-size: 16px;"
                                "   border: 2px solid #16a085;"
                                "   border-radius: 8px;"
                                "   padding: 8px 16px;"
                                "}"
                                "QPushButton:hover {"
                                "   background-color: #16a085;"
                                "}"
                                "QPushButton:pressed {"
                                "   background-color: #148f77;"
                                "   border: 2px solid #117864;"
                                "}");
        
        // 布局
        layout = new QVBoxLayout();
        layout->addWidget(backButton);
        layout->setAlignment(Qt::AlignVCenter);
        mainLayout->addLayout(layout);
        mainLayout->setAlignment(Qt::AlignCenter);

        // 信号槽：点击返回按钮发送信号
        connect(backButton, &QPushButton::clicked, this, &ConfigPage::configCompleted);
    }

signals:
    void configCompleted(); // 配置完成信号

    
};

class View : public QMainWindow {
  Q_OBJECT
  private:
    int height;
    int width;
    Model model;
    //using dynamic_cast to change type
    std::unordered_map<string,QObject*> qObjects;
    std::unordered_map<string,QString*> qStrings;

    ConfigPage* configPage;

    void warnUser(const std::string& content) {
      // 将 std::string 转换为 QString
      QString qContent = createQString(content);

      // 显示警告框
      QMessageBox::warning(nullptr, createQString("Warning"), qContent);
  }

  public:
    explicit View(Model &model, QWidget *parent = nullptr,int width=1500,int height=750);
    ~View();

    QObject* getQObject(string name){
      return qObjects.at(name);
    }

    void setPathLabelContent(string content){
      QLabel* pathLabel=dynamic_cast<QLabel*>(qObjects["pathLabel"]);
      pathLabel->setText(createQString(content));
    }

    QString& createQString(const string& content){
      if(qStrings.find(content)==qStrings.end()){
        QString * newString=new QString(content.c_str());
        qStrings[content]=newString;
        return (*newString);
      }else{
        return getQString(content);
      }
    }

    QString& createQString(const char* content){
      if(qStrings.find(content)==qStrings.end()){
        QString * newString=new QString(content);
        qStrings[content]=newString;
        return (*newString);
      }else{
        return getQString(content);
      }
    }

    QString& getQString(string content){
      return (*qStrings[content]);
    }

    void removeQString(string content){
      auto result=qStrings.extract(content).mapped();
      delete result;
    }

    string fromQString(const QString& qString){
      std::string result;

      // 遍历 QString 的迭代器
      for (auto it = qString.begin(); it != qString.end(); ++it) {
          QChar ch = *it;

          // 根据需求选择转换方法，例如 toLatin1 或 Unicode 转换
          result += ch.toLatin1(); // 或者使用 ch.unicode() 进行更高级别的处理
      }

      return result;
    }

    pair<string,string> getStringFromTextField(){
        QLineEdit* editField1=dynamic_cast<QLineEdit*>(qObjects["location1Input"]);
        QLineEdit* editField2=dynamic_cast<QLineEdit*>(qObjects["location2Input"]);
        const QString& text_1=editField1->text();
        const QString& text_2=editField2->text();
        string text_1_std=fromQString(text_1);
        string text_2_std=fromQString(text_2);
        return {text_1_std,text_2_std};
    }



  private slots:
    void onButtonClicked(); 

    void openConfigPage() {             // 隐藏主页面
        configPage->show();        // 显示配置页面
    }

    void showMainPage() {
        configPage->hide();        // 隐藏配置页面           // 显示主页面
    }
};



#endif /* F0C98FE3_6EA2_41DD_87A2_BCF91456C0BE */
