#include "View.hpp"

class ConfigPage;

// View.cpp
View::View(Model &model, QWidget *parent, int width, int height) : QMainWindow(parent), model(model) {
    // 设置窗口标题和大小
    setWindowTitle("Map System");
    resize(width, height);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    qObjects["centralWidget"] = centralWidget;

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    qObjects["mainLayout"] = mainLayout;

    QVBoxLayout *configButtonLayout = new QVBoxLayout();
    QPushButton *configButton = new QPushButton("Config", this);
    qObjects["configButtonLayout"] = configButtonLayout;
    qObjects["configButton"] = configButton;
    configButton->setStyleSheet("QPushButton {"
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
    configButton->setFixedSize(100, 50);
    configButtonLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    configButtonLayout->addWidget(configButton);
    mainLayout->addLayout(configButtonLayout);

    // 配置页面
    configPage = new ConfigPage();

    // 信号槽：点击按钮显示配置页面
    connect(configButton, &QPushButton::clicked, this, &View::openConfigPage);

    // 信号槽：从配置页面返回主页面
    connect(configPage, &ConfigPage::configCompleted, this, &View::showMainPage);

    // 左侧地图区域
    QLabel *mapLabel = new QLabel(createQString("Map Area"), this);
    qObjects["mapLabel"] = mapLabel;

    QPixmap mapPixmap(createQString("./resources/tagged.png"));

    mapLabel->setPixmap(mapPixmap);
    mapLabel->setScaledContents(true); // 启用自动缩放
    mapLabel->setFixedSize(900, 550);
    mapLabel->setStyleSheet("background-color: lightgray; border: 1px solid black;");
    mainLayout->addWidget(mapLabel);

    // 右侧控件区域布局
    QVBoxLayout *rightLayout = new QVBoxLayout();
    qObjects["rightLayout"] = rightLayout;

    QLabel *titleLabel = new QLabel("Map System", this);
    qObjects["titleLabel"] = titleLabel;
    titleLabel->setStyleSheet("font-size: 25px; font-weight: bold; margin-bottom: 2px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(titleLabel);

    QHBoxLayout *inputLayout = new QHBoxLayout(); // 改为水平布局
    qObjects["inputLayout"] = inputLayout;
    inputLayout->setAlignment(Qt::AlignTop); // 将输入框部分靠近顶部

    // 创建第一个输入框
    QVBoxLayout *location1Layout = new QVBoxLayout(); // 嵌套一个垂直布局，用于标签和输入框的组合
    QLabel *location1Label = new QLabel("Location 1:", this);
    location1Label->setStyleSheet("font-size: 12px; font-weight: bold; margin-bottom: 2px;");
    QLineEdit *location1Input = new QLineEdit(this);

    location1Input->setFixedSize(160, 40); // 调整输入框大小
    location1Layout->addWidget(location1Label);
    location1Layout->addWidget(location1Input);
    qObjects["location1Layout"] = location1Layout;
    qObjects["location1Label"] = location1Label;
    qObjects["location1Input"] = location1Input;

    // 创建第二个输入框
    QVBoxLayout *location2Layout = new QVBoxLayout(); // 嵌套另一个垂直布局
    QLabel *location2Label = new QLabel("Location 2:", this);
    location2Label->setStyleSheet("font-size: 12px; font-weight: bold; margin-bottom: 2px;");
    QLineEdit *location2Input = new QLineEdit(this);
    location2Input->setFixedSize(160, 40); // 调整输入框大小
    location2Layout->addWidget(location2Label);
    location2Layout->addWidget(location2Input);
    qObjects["location2Layout"] = location2Layout;
    qObjects["location2Label"] = location2Label;
    qObjects["location2Input"] = location2Input;
    location1Input->setStyleSheet("QLineEdit {"
                                  "   border: 2px solid #3498db;"
                                  "   border-radius: 5px;"
                                  "   padding: 5px;"
                                  "   background-color: #ecf0f1;"
                                  "   font-size: 16px;"
                                  "}"
                                  "QLineEdit:focus {"
                                  "   border: 2px solid #2980b9;"
                                  "   background-color: #ffffff;"
                                  "}");
    location2Input->setStyleSheet("QLineEdit {"
                                  "   border: 2px solid #3498db;"
                                  "   border-radius: 5px;"
                                  "   padding: 5px;"
                                  "   background-color: #ecf0f1;"
                                  "   font-size: 16px;"
                                  "}"
                                  "QLineEdit:focus {"
                                  "   border: 2px solid #2980b9;"
                                  "   background-color: #ffffff;"
                                  "}");

    // 将两个垂直布局加入水平布局
    inputLayout->addLayout(location1Layout);
    inputLayout->addLayout(location2Layout);

    inputLayout->setAlignment(Qt::AlignHCenter);
    // 将输入框布局添加到右侧总布局
    rightLayout->addLayout(inputLayout);

    // 添加其余控件（操作按钮和路径显示）
    // 添加其余控件（操作按钮和路径显示）
    QGridLayout *buttonGridLayout = new QGridLayout();
    qObjects["buttonGridLayout"] = buttonGridLayout;

    // 创建按钮并添加到网格布局
    for (int i = 0; i < 4; ++i) {

        string name = "Operation " + std::to_string(i + 1);
        QPushButton *button = new QPushButton(createQString(name), this);
        button->setStyleSheet("QPushButton {"
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
        button->setObjectName(QString::fromStdString(name));
        qObjects[name] = button;
        button->setFixedSize(200, 65);                 // 按钮大小
        int row = i / 2;                               // 计算按钮所在的行
        int col = i % 2;                               // 计算按钮所在的列
        buttonGridLayout->addWidget(button, row, col); // 添加按钮到网格布局
    }

    QPushButton *button0 = dynamic_cast<QPushButton *>(qObjects["Operation 1"]);
    connect(button0, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        try {
            auto result = this->model.operation_I(targets.first, targets.second);
            string text = this->model.getMap().shortestPathOneToString(result, targets.second);
            this->setPathLabelContent(text);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button1 = dynamic_cast<QPushButton *>(qObjects["Operation 2"]);
    connect(button1, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        try {
            auto result = this->model.operation_II(targets.first);
            string text = this->model.getMap().shortestPathsToString(result);
            this->setPathLabelContent(text);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button2 = dynamic_cast<QPushButton *>(qObjects["Operation 3"]);
    connect(button2, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        try {
            if (targets.first.empty()) {
                targets.first = "A";
            }
            auto result = this->model.operation_III(targets.first);
            string text = this->model.getMap().mstToString(result);
            this->setPathLabelContent(text);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button3 = dynamic_cast<QPushButton *>(qObjects["Operation 4"]);
    connect(button3, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        try {
            auto result = this->model.operation_IV(targets.first);
            string text = this->model.getMap().shortestPathsToStringSingleStyle(result);
            this->setPathLabelContent(text);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });

    // 将网格布局添加到右侧布局
    rightLayout->addLayout(buttonGridLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    qObjects["scrollArea"] = scrollArea;
    scrollArea->setWidgetResizable(true); // 允许滚动区域调整内容大小
    scrollArea->setFixedSize(465, 465);
    // 创建路径标签
    QLabel *pathLabel = new QLabel("路径: A --> B\n距离: 2.5 km", this);
    qObjects["pathLabel"] = pathLabel;
    pathLabel->setStyleSheet("border: 1px solid black; background-color: white; font-size: 12px;");
    pathLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft); // 设置文本顶部左对齐
    pathLabel->setWordWrap(true);                          // 启用自动换行
    pathLabel->setMinimumSize(450, 450);                   // 设置宽度限制，避免内容溢出
    pathLabel->setText("example");
    pathLabel->setStyleSheet("QLabel {"
                             "   color: #2c3e50;"
                             "   font-size: 15px;"
                             "   font-weight: bold;"
                             "   border: 1px solid #bdc3c7;"
                             "   background-color: #ecf0f1;"
                             "   padding: 5px;"
                             "   border-radius: 5px;"
                             "}");    // 添加更多文本
    scrollArea->setWidget(pathLabel); // 将 QLabel 设置为 QScrollArea 的子组件

    // 创建包装布局
    QVBoxLayout *pathWrapLayout = new QVBoxLayout();
    qObjects["pathWrapLayout"] = pathWrapLayout;
    pathWrapLayout->setAlignment(Qt::AlignCenter);
    pathWrapLayout->addWidget(scrollArea); // 添加滚动区域到布局中

    rightLayout->addLayout(pathWrapLayout);
    rightLayout->addStretch(); // 拉伸以占满剩余空间

    // 将右侧布局添加到主布局
    mainLayout->addLayout(rightLayout);
}
// 初始化代码

View::~View() {
    // 清理代码
}

void View::onButtonClicked() {
    // 获取发送信号的对象

    QPushButton *senderButton = qobject_cast<QPushButton *>(sender());
    if (senderButton) {
        // 根据按钮的对象名称或文本区分不同按钮
        QString buttonName = senderButton->objectName();
        qDebug() << "Button clicked:" << buttonName;

        if (buttonName == "OperationButton0") {
            qDebug() << "Operation 1 executed";
        } else if (buttonName == "OperationButton1") {
            qDebug() << "Operation 2 executed";
        } else if (buttonName == "OperationButton2") {
            qDebug() << "Operation 3 executed";
        } else if (buttonName == "OperationButton3") {
            qDebug() << "Operation 4 executed";
        }
    }
}