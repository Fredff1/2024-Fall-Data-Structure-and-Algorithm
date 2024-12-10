#include "View.hpp"

class ConfigPage;

ConfigPage::ConfigPage(View *view, QWidget *parent) : QWidget(parent), view(view) {
    setWindowTitle("Config Page");
    setFixedSize(250, 400);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    mainLayout = new QHBoxLayout(this);
    layout = new QVBoxLayout();

    std::vector<std::string> operations = {"Operation 1", "Operation 2", "Operation 3", "Operation 4"};
    for (const auto &operation : operations) {
        QLabel *label = new QLabel(createQString(operation), this);
        label->setStyleSheet("QLabel {"
                             "   color: #2c3e50;"
                             "   font-size: 10px;"
                             "   font-weight: bold;"
                             "   border: 1px solid #bdc3c7;"
                             "   background-color: #ecf0f1;"
                             "   padding: 5px;"
                             "   border-radius: 5px;"
                             "}");
        QComboBox *comboBox = new QComboBox(this);

        comboBox->setStyleSheet("QComboBox {"
                                "   background-color: #1abc9c;"
                                "   color: white;"
                                "   font-size: 16px;"
                                "   border: 2px solid #16a085;"
                                "   border-radius: 8px;"
                                "   padding: 8px 16px;"
                                "} "
                                "QComboBox::drop-down {"
                                "   border: 0px;"
                                "} "
                                "QComboBox::down-arrow {"
                                "   image: url(:/icons/arrow_down.png);"
                                "   width: 14px;"
                                "   height: 14px;"
                                "} "
                                "QComboBox:hover {"
                                "   background-color: #16a085;"
                                "} "
                                "QComboBox:pressed {"
                                "   background-color: #148f77;"
                                "   border: 2px solid #117864;"
                                "} "
                                "QComboBox QAbstractItemView {"
                                "   background-color: #1abc9c;"
                                "   color: white;"
                                "   border: 1px solid #16a085;"
                                "} ");

        initOperationAlgorithmTypes(operation, comboBox);
        operationComboBoxes[operation] = comboBox;

        layout->addWidget(label);
        layout->addWidget(comboBox);

        connect(comboBox, &QComboBox::currentTextChanged, [this, operation](const QString &selectedAlgorithm) {
            string type = fromQString(selectedAlgorithm);
            this->view->setOperationAlgorithm(operation, type);
        });
    }

    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet("QPushButton {"
                              "   background-color: #1abc9c;"
                              "   color: white;"
                              "   font-size: 16px;"
                              "   border: 2px solid #16a085;"
                              "   border-radius: 8px;"
                              "   padding: 8px 16px;"
                              "} "
                              "QPushButton:hover {"
                              "   background-color: #16a085;"
                              "} "
                              "QPushButton:pressed {"
                              "   background-color: #148f77;"
                              "   border: 2px solid #117864;"
                              "}");

    layout->addWidget(backButton);
    layout->setAlignment(Qt::AlignVCenter);
    mainLayout->addLayout(layout);
    mainLayout->setAlignment(Qt::AlignCenter);

    connect(backButton, &QPushButton::clicked, this, &ConfigPage::configCompleted);
}

void ConfigPage::initOperationAlgorithmTypes(string operation_name, QComboBox *comboBox) {
    if (operation_name == "Operation 1") {
        comboBox->addItem(createQString("bellmanFord"));
        comboBox->addItem(createQString("dijkstra"));
        view->setOperationAlgorithm("Operation 1", "bellmanFord");

    } else if (operation_name == "Operation 2") {
        comboBox->addItem(createQString("bellmanFord"));
        comboBox->addItem(createQString("dijkstra"));
        view->setOperationAlgorithm("Operation 2", "bellmanFord");
    } else if (operation_name == "Operation 3") {
        comboBox->addItem(createQString("prim"));
        comboBox->addItem(createQString("kruskal"));
        view->setOperationAlgorithm("Operation 3", "prim");
    } else if (operation_name == "Operation 4") {
        comboBox->addItem(createQString("dijkstra"));
        comboBox->addItem(createQString("bellmanFord"));
        view->setOperationAlgorithm("Operation 4", "dijkstra");
    }
}

// View.cpp
View::View(Model &model, QWidget *parent, int width, int height) : QMainWindow(parent), model(model) {

    setWindowTitle("Map System");
    resize(width, height);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    qObjects["centralWidget"] = centralWidget;

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
    QPushButton *exitButton = new QPushButton("Exit", this);
    qObjects["exitButton"] = exitButton;
    exitButton->setStyleSheet("QPushButton {"
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
    configButtonLayout->addWidget(exitButton);
    mainLayout->addLayout(configButtonLayout);

    configPage = new ConfigPage(this);

    connect(configButton, &QPushButton::clicked, this, &View::openConfigPage);

    connect(exitButton, &QPushButton::clicked, this, &QApplication::exit);

    connect(configPage, &ConfigPage::configCompleted, this, &View::showMainPage);

    QLabel *mapLabel = new QLabel(createQString("Map Area"), this);
    qObjects["mapLabel"] = mapLabel;

    QPixmap mapPixmap(createQString("./resources/tagged.png"));

    mapLabel->setPixmap(mapPixmap);
    mapLabel->setScaledContents(true);
    mapLabel->setFixedSize(900, 550);
    mapLabel->setStyleSheet("background-color: lightgray; border: 1px solid black;");
    mainLayout->addWidget(mapLabel);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    qObjects["rightLayout"] = rightLayout;

    QLabel *titleLabel = new QLabel("Map System", this);
    qObjects["titleLabel"] = titleLabel;
    titleLabel->setStyleSheet("font-size: 25px; font-weight: bold; margin-bottom: 2px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(titleLabel);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    qObjects["inputLayout"] = inputLayout;
    inputLayout->setAlignment(Qt::AlignTop);

    QVBoxLayout *location1Layout = new QVBoxLayout();
    QLabel *location1Label = new QLabel("Location 1:", this);
    location1Label->setStyleSheet("font-size: 12px; font-weight: bold; margin-bottom: 2px;");
    QLineEdit *location1Input = new QLineEdit(this);

    location1Input->setFixedSize(160, 40);
    location1Layout->addWidget(location1Label);
    location1Layout->addWidget(location1Input);
    qObjects["location1Layout"] = location1Layout;
    qObjects["location1Label"] = location1Label;
    qObjects["location1Input"] = location1Input;

    QVBoxLayout *location2Layout = new QVBoxLayout();
    QLabel *location2Label = new QLabel("Location 2:", this);
    location2Label->setStyleSheet("font-size: 12px; font-weight: bold; margin-bottom: 2px;");
    QLineEdit *location2Input = new QLineEdit(this);
    location2Input->setFixedSize(160, 40);
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

    inputLayout->addLayout(location1Layout);
    inputLayout->addLayout(location2Layout);

    inputLayout->setAlignment(Qt::AlignHCenter);

    rightLayout->addLayout(inputLayout);

    QGridLayout *buttonGridLayout = new QGridLayout();
    qObjects["buttonGridLayout"] = buttonGridLayout;

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
        button->setFixedSize(200, 65);
        int row = i / 2;
        int col = i % 2;
        buttonGridLayout->addWidget(button, row, col);
    }

    QPushButton *button0 = dynamic_cast<QPushButton *>(qObjects["Operation 1"]);
    connect(button0, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        string type = this->algorithmTypes["Operation 1"];
        try {
            string result = this->model.operation_I(targets.first, targets.second, type);

            this->setPathLabelContent(result);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button1 = dynamic_cast<QPushButton *>(qObjects["Operation 2"]);
    connect(button1, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        string type = this->algorithmTypes["Operation 2"];
        try {
            string result = this->model.operation_II(targets.first, type);

            this->setPathLabelContent(result);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button2 = dynamic_cast<QPushButton *>(qObjects["Operation 3"]);
    connect(button2, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        string type = this->algorithmTypes["Operation 3"];
        try {
            if (targets.first.empty()) {
                targets.first = "A";
            }
            string result = this->model.operation_III(targets.first, type);
            this->setPathLabelContent(result);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });
    QPushButton *button3 = dynamic_cast<QPushButton *>(qObjects["Operation 4"]);
    connect(button3, &QPushButton::clicked, this, [this]() {
        auto targets = getStringFromTextField();
        string type = this->algorithmTypes["Operation 4"];
        try {
            string result = this->model.operation_IV(targets.first, type);

            this->setPathLabelContent(result);
        } catch (std::exception &e) {
            this->warnUser("Invalid input");
        }
    });

    rightLayout->addLayout(buttonGridLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    qObjects["scrollArea"] = scrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(465, 465);

    QLabel *pathLabel = new QLabel("路径: A --> B\n距离: 2.5 km", this);
    qObjects["pathLabel"] = pathLabel;
    pathLabel->setStyleSheet("border: 1px solid black; background-color: white; font-size: 12px;");
    pathLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    pathLabel->setWordWrap(true);
    pathLabel->setMinimumSize(450, 450);
    pathLabel->setText("example");
    pathLabel->setStyleSheet("QLabel {"
                             "   color: #2c3e50;"
                             "   font-size: 15px;"
                             "   font-weight: bold;"
                             "   border: 1px solid #bdc3c7;"
                             "   background-color: #ecf0f1;"
                             "   padding: 5px;"
                             "   border-radius: 5px;"
                             "}");
    scrollArea->setWidget(pathLabel);

    QVBoxLayout *pathWrapLayout = new QVBoxLayout();
    qObjects["pathWrapLayout"] = pathWrapLayout;
    pathWrapLayout->setAlignment(Qt::AlignCenter);
    pathWrapLayout->addWidget(scrollArea);

    rightLayout->addLayout(pathWrapLayout);
    rightLayout->addStretch();

    mainLayout->addLayout(rightLayout);
}

View::~View() {
}
