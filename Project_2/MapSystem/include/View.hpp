#ifndef VIEW_HPP
#define VIEW_HPP
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <string>

#include "Model.hpp"
#include "GlobalFlags.hpp"

class View;

class MyQStringHandler{
  private:
    std::unordered_map<string, QString *> qStrings;
  public:
  ~MyQStringHandler(){
   
  }
  QString &createQString(const string &content) {
        if (qStrings.find(content) == qStrings.end()) {
            QString *newString = new QString(content.c_str());
            qStrings[content] = newString;
            return (*newString);
        } else {
            return getQString(content);
        }
    }

    QString &createQString(const char *content) {
        if (qStrings.find(content) == qStrings.end()) {
            QString *newString = new QString(content);
            qStrings[content] = newString;
            return (*newString);
        } else {
            return getQString(content);
        }
    }

    QString &getQString(const string& content) {
        return (*qStrings[content]);
    }

    void removeQString(const string& content) {
        auto result = qStrings.extract(content).mapped();
        delete result;
    }

    string fromQString(const QString &qString) {
        std::string result;
        for (auto it = qString.begin(); it != qString.end(); ++it) {
            QChar ch = *it;
            result += ch.toLatin1(); 
        }

        return result;
    }
};

class ConfigPage : public QWidget,MyQStringHandler{
    Q_OBJECT
  private:
    QPushButton *backButton;
    QVBoxLayout *layout;
    QHBoxLayout *mainLayout;
    View *view;
    std::unordered_map<std::string, QComboBox *> operationComboBoxes; 

  public:
    ConfigPage(View *view, QWidget *parent = nullptr);

    void initOperationAlgorithmTypes(string operation_name,QComboBox *comboBox);
    

  signals:
    void configCompleted(); 
};

class View : public QMainWindow ,MyQStringHandler {
    Q_OBJECT
  private:
    int height;
    int width;
    Model model;
 
    std::unordered_map<string, QObject *> qObjects;
    std::unordered_map<string, string> algorithmTypes;

    ConfigPage *configPage;

    void warnUser(const std::string &content) {
       
        QString qContent = createQString(content);

     
        QMessageBox::warning(nullptr, createQString("Warning"), qContent);
    }

  public:
    explicit View(Model &model, QWidget *parent = nullptr, int width = 1500, int height = 750);
    ~View();

    void setOperationAlgorithm(string operation_name, string algorithm_name) {
        algorithmTypes[operation_name] = algorithm_name;
    }

    QObject *getQObject(string name) {
        return qObjects.at(name);
    }

    void setPathLabelContent(string content) {
        QLabel *pathLabel = dynamic_cast<QLabel *>(qObjects["pathLabel"]);
        pathLabel->setText(createQString(content));
    }

    

    pair<string, string> getStringFromTextField() {
        QLineEdit *editField1 = dynamic_cast<QLineEdit *>(qObjects["location1Input"]);
        QLineEdit *editField2 = dynamic_cast<QLineEdit *>(qObjects["location2Input"]);
        const QString &text_1 = editField1->text();
        const QString &text_2 = editField2->text();
        string text_1_std = fromQString(text_1);
        string text_2_std = fromQString(text_2);
        return {text_1_std, text_2_std};
    }

  private slots:
    

    void openConfigPage() { 
        configPage->show(); 
    }

    void showMainPage() {
        configPage->hide(); 
    }
};

#endif 
