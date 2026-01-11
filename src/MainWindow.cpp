/**
 * @file MainWindow.cpp
 * @brief Implementation of the main GUI window logic.
 */

#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QFormLayout>
#include <QRegularExpression>
#include <algorithm>
#include <random>
#include <sstream>
#include <vector>

using std::vector;
using std::string;
using std::stringstream;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("12306 Train Ticket Simulation System");
    resize(1000, 700);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setupUi();
}

MainWindow::~MainWindow() {
}

/**
 * @brief Sets up all the UI pages.
 */
void MainWindow::setupUi() {
    createLoginPage();
    createRegisterPage();
    createPassengerPage();
    createAdminPage();

    stackedWidget->setCurrentIndex(0); // Start at Login
}

/**
 * @brief Creates the login page layout.
 */
void MainWindow::createLoginPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    QGroupBox *groupBox = new QGroupBox("Login");
    groupBox->setFixedSize(400, 300);
    QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);

    loginUsernameInput = new QLineEdit();
    loginUsernameInput->setPlaceholderText("Username");
    loginPasswordInput = new QLineEdit();
    loginPasswordInput->setPlaceholderText("Password");
    loginPasswordInput->setEchoMode(QLineEdit::Password);

    QPushButton *loginBtn = new QPushButton("Login");
    QPushButton *regBtn = new QPushButton("Register New Account");

    groupLayout->addWidget(new QLabel("Username:"));
    groupLayout->addWidget(loginUsernameInput);
    groupLayout->addWidget(new QLabel("Password:"));
    groupLayout->addWidget(loginPasswordInput);
    groupLayout->addSpacing(20);
    groupLayout->addWidget(loginBtn);
    groupLayout->addWidget(regBtn);

    layout->addWidget(groupBox);

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(regBtn, &QPushButton::clicked, this, &MainWindow::showRegisterPage);

    stackedWidget->addWidget(page);
}

/**
 * @brief Creates the registration page layout.
 */
void MainWindow::createRegisterPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    QGroupBox *groupBox = new QGroupBox("Register");
    groupBox->setFixedSize(400, 400);
    QFormLayout *formLayout = new QFormLayout(groupBox);

    regUsernameInput = new QLineEdit();
    regPasswordInput = new QLineEdit();
    regPasswordInput->setEchoMode(QLineEdit::Password);
    regNameInput = new QLineEdit();
    regIdInput = new QLineEdit();

    formLayout->addRow("Username:", regUsernameInput);
    formLayout->addRow("Password:", regPasswordInput);
    formLayout->addRow("Real Name:", regNameInput);
    formLayout->addRow("ID Card:", regIdInput);

    QPushButton *submitBtn = new QPushButton("Submit Registration");
    QPushButton *backBtn = new QPushButton("Back to Login");

    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(backBtn);
    
    formLayout->addRow(btnLayout);

    layout->addWidget(groupBox);

    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::handleRegister);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showLoginPage);

    stackedWidget->addWidget(page);
}

/**
 * @brief Creates the passenger dashboard.
 */
void MainWindow::createPassengerPage() {
    passengerPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(passengerPage);

    // Top Bar (Welcome + Logout)
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(new QLabel("Welcome, Passenger!"));
    topBar->addStretch();
    QPushButton *logoutBtn = new QPushButton("Logout");
    topBar->addWidget(logoutBtn);
    mainLayout->addLayout(topBar);

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);

    // Search Area
    QGroupBox *searchGroup = new QGroupBox("Search Trains");
    QHBoxLayout *searchLayout = new QHBoxLayout(searchGroup);
    
    searchStartInput = new QLineEdit();
    searchStartInput->setPlaceholderText("From Station");
    searchEndInput = new QLineEdit();
    searchEndInput->setPlaceholderText("To Station");
    searchDateInput = new QDateEdit(QDate::currentDate());
    searchDateInput->setDisplayFormat("yyyy-MM-dd");
    
    QPushButton *searchBtn = new QPushButton("Search");
    
    searchLayout->addWidget(new QLabel("From:"));
    searchLayout->addWidget(searchStartInput);
    searchLayout->addWidget(new QLabel("To:"));
    searchLayout->addWidget(searchEndInput);
    searchLayout->addWidget(new QLabel("Date:"));
    searchLayout->addWidget(searchDateInput);
    searchLayout->addWidget(searchBtn);
    
    mainLayout->addWidget(searchGroup);

    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::handleSearch);

    // Results Table
    trainResultTable = new QTableWidget();
    trainResultTable->setColumnCount(6);
    trainResultTable->setHorizontalHeaderLabels({"Train ID", "Type", "Start Time", "End Time", "Price", "Action"});
    trainResultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(trainResultTable);

    // Order History
    QGroupBox *historyGroup = new QGroupBox("My Orders");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);
    
    orderHistoryTable = new QTableWidget();
    orderHistoryTable->setColumnCount(7);
    orderHistoryTable->setHorizontalHeaderLabels({"Order ID", "Train", "Route", "Date", "Seats", "Status", "Action"});
    orderHistoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    historyLayout->addWidget(orderHistoryTable);
    mainLayout->addWidget(historyGroup);

    stackedWidget->addWidget(passengerPage);
}

/**
 * @brief Creates the admin dashboard.
 */
void MainWindow::createAdminPage() {
    adminPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(adminPage);

    // Top Bar
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(new QLabel("Admin Dashboard"));
    topBar->addStretch();
    QPushButton *logoutBtn = new QPushButton("Logout");
    topBar->addWidget(logoutBtn);
    mainLayout->addLayout(topBar);

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::handleLogout);

    // Train Management
    QGroupBox *trainGroup = new QGroupBox("All Trains");
    QVBoxLayout *trainLayout = new QVBoxLayout(trainGroup);
    
    allTrainsTable = new QTableWidget();
    allTrainsTable->setColumnCount(4);
    allTrainsTable->setHorizontalHeaderLabels({"Train ID", "Type", "Total Seats", "Route Info"});
    allTrainsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    trainLayout->addWidget(allTrainsTable);
    
    // Simple Add Train UI (Simplified for demo)
    QHBoxLayout *addLayout = new QHBoxLayout();
    addTrainIdInput = new QLineEdit(); addTrainIdInput->setPlaceholderText("Train ID");
    addTrainTypeInput = new QLineEdit(); addTrainTypeInput->setPlaceholderText("Type");
    addTrainSeatsInput = new QLineEdit(); addTrainSeatsInput->setPlaceholderText("Seats");
    
    // Route Selection
    addTrainRouteMode = new QComboBox();
    addTrainRouteMode->addItem("Default Route (Beijing->Shanghai)");
    addTrainRouteMode->addItem("Random Route");
    addTrainRouteMode->addItem("Custom Route");
    
    addTrainCustomRouteInput = new QLineEdit();
    addTrainCustomRouteInput->setPlaceholderText("e.g. A->B->C (Custom Only)");
    addTrainCustomRouteInput->setEnabled(false);

    connect(addTrainRouteMode, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        addTrainCustomRouteInput->setEnabled(index == 2);
    });

    QPushButton *addBtn = new QPushButton("Add Train (Demo)");
    
    addLayout->addWidget(addTrainIdInput);
    addLayout->addWidget(addTrainTypeInput);
    addLayout->addWidget(addTrainSeatsInput);
    addLayout->addWidget(addTrainRouteMode);
    addLayout->addWidget(addTrainCustomRouteInput);
    addLayout->addWidget(addBtn);
    
    trainLayout->addLayout(addLayout);
    mainLayout->addWidget(trainGroup);
    
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::handleAddTrain);

    stackedWidget->addWidget(adminPage);
}

// Slots Implementation

void MainWindow::showRegisterPage() {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showLoginPage() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::handleLogin() {
    QString username = loginUsernameInput->text();
    QString password = loginPasswordInput->text();

    auto user = systemManager.login(username.toStdString(), password.toStdString());
    if (user) {
        if (user->getRole() == "Admin") {
            updateAdminView();
            stackedWidget->setCurrentWidget(adminPage);
        } else {
            updatePassengerView();
            stackedWidget->setCurrentWidget(passengerPage);
        }
        // Clear inputs
        loginUsernameInput->clear();
        loginPasswordInput->clear();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
    }
}

void MainWindow::handleLogout() {
    systemManager.logout();
    showLoginPage();
}

void MainWindow::handleRegister() {
    QString u = regUsernameInput->text();
    QString p = regPasswordInput->text();
    QString n = regNameInput->text();
    QString i = regIdInput->text();

    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and Password required");
        return;
    }

    if (systemManager.registerUser(u.toStdString(), p.toStdString(), n.toStdString(), i.toStdString())) {
        QMessageBox::information(this, "Success", "Registration Successful");
        showLoginPage();
    } else {
        QMessageBox::warning(this, "Error", "Username already exists");
    }
}

void MainWindow::handleSearch() {
    QString start = searchStartInput->text();
    QString end = searchEndInput->text();
    QString date = searchDateInput->date().toString("yyyy-MM-dd");

    if (start.isEmpty() || end.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter start and end stations");
        return;
    }

    vector<Train> results = systemManager.searchTrains(start.toStdString(), end.toStdString(), date.toStdString());

    trainResultTable->setRowCount(0);
    for (auto& train : results) {
        int row = trainResultTable->rowCount();
        trainResultTable->insertRow(row);
        
        trainResultTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(train.getId())));
        trainResultTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(train.getType())));
        trainResultTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(train.getDepartureTime(start.toStdString()))));
        trainResultTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(train.getArrivalTime(end.toStdString()))));
        
        double price = train.getPrice(start.toStdString(), end.toStdString());
        trainResultTable->setItem(row, 4, new QTableWidgetItem(QString::number(price)));

        QPushButton *bookBtn = new QPushButton("Book");
        connect(bookBtn, &QPushButton::clicked, [=]() {
            if (systemManager.bookTicket(train.getId(), start.toStdString(), end.toStdString(), date.toStdString())) {
                QMessageBox::information(this, "Success", "Ticket Booked Successfully!");
                refreshOrderTable();
            } else {
                QMessageBox::warning(this, "Failed", "Booking Failed (No seats or error)");
            }
        });
        trainResultTable->setCellWidget(row, 5, bookBtn);
    }
    
    if (results.empty()) {
        QMessageBox::information(this, "No Results", "No trains found for this route.");
    }
}

void MainWindow::refreshOrderTable() {
    auto user = systemManager.getCurrentUser();
    if (!user || user->getRole() != "Passenger") return;

    Passenger* p = dynamic_cast<Passenger*>(user.get());
    vector<Order>& orders = p->getOrders();

    orderHistoryTable->setRowCount(0);
    for (auto& order : orders) {
        int row = orderHistoryTable->rowCount();
        orderHistoryTable->insertRow(row);

        orderHistoryTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(order.getOrderId())));
        orderHistoryTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(order.getTrainId())));
        orderHistoryTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(order.getStartStation() + "->" + order.getEndStation())));
        orderHistoryTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(order.getDate())));
        orderHistoryTable->setItem(row, 4, new QTableWidgetItem(QString::number(order.getTicketCount())));
        
        QString statusStr = (order.getStatus() == PAID) ? "Paid" : (order.getStatus() == CANCELLED ? "Cancelled" : "Completed");
        orderHistoryTable->setItem(row, 5, new QTableWidgetItem(statusStr));

        if (order.getStatus() == PAID) {
            QPushButton *refundBtn = new QPushButton("Refund");
            connect(refundBtn, &QPushButton::clicked, [=]() {
                if (systemManager.refundTicket(order.getOrderId())) {
                    QMessageBox::information(this, "Success", "Refund Successful");
                    refreshOrderTable();
                } else {
                    QMessageBox::warning(this, "Error", "Refund Failed");
                }
            });
            orderHistoryTable->setCellWidget(row, 6, refundBtn);
        } else {
             orderHistoryTable->setItem(row, 6, new QTableWidgetItem("-"));
        }
    }
}

void MainWindow::handleAddTrain() {
    QString id = addTrainIdInput->text();
    QString type = addTrainTypeInput->text();
    int seats = addTrainSeatsInput->text().toInt();
    int mode = addTrainRouteMode->currentIndex();

    if (id.isEmpty()) return;

    Train t(id.toStdString(), type.toStdString(), seats);
    
    if (mode == 0) { // Default
        t.addStop({"Beijing", "08:00", "08:00", 0.0, 0});
        t.addStop({"Shanghai", "13:00", "13:00", 500.0, 1300});
    } 
    else if (mode == 1) { // Random
        vector<string> cities = {"Beijing", "Shanghai", "Guangzhou", "Shenzhen", "Chengdu", "Xian", "Wuhan", "Nanjing", "Hangzhou", "Tianjin"};
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cities.begin(), cities.end(), g);
        
        int numStops = 2 + (rd() % 4); // 2 to 5 stops
        
        int currentHour = 8;
        int currentDist = 0;
        double currentPrice = 0.0;
        
        for (int i = 0; i < numStops; ++i) {
            string arrTime = (currentHour < 10 ? "0" : "") + std::to_string(currentHour) + ":00";
            string depTime = arrTime; // Simplified: arrive and depart same time
            
            t.addStop({cities[i], arrTime, depTime, currentPrice, currentDist});
            
            currentHour += 2 + (rd() % 3); // +2 to 4 hours
            currentDist += 300 + (rd() % 500);
            currentPrice += 100.0 + (rd() % 200);
        }
    }
    else if (mode == 2) { // Custom
        QString customStr = addTrainCustomRouteInput->text();
        if (customStr.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a custom route");
            return;
        }
        
        // Split by -> or - or , or space
        QStringList stations = customStr.split(QRegularExpression("->|-|,|\\s+"), Qt::SkipEmptyParts);
        
        if (stations.size() < 2) {
            QMessageBox::warning(this, "Error", "Route must have at least 2 stations");
            return;
        }
        
        int currentHour = 8;
        int currentDist = 0;
        double currentPrice = 0.0;
        
        for (int i = 0; i < stations.size(); ++i) {
            string arrTime = (currentHour < 10 ? "0" : "") + std::to_string(currentHour) + ":00";
            string depTime = arrTime;
            
            t.addStop({stations[i].toStdString(), arrTime, depTime, currentPrice, currentDist});
            
            currentHour += 3; // Fixed 3 hours between stations
            currentDist += 400;
            currentPrice += 150.0;
        }
    }
    
    systemManager.addTrain(t);
    refreshTrainTable();
    QMessageBox::information(this, "Success", "Train Added Successfully");
}

void MainWindow::refreshTrainTable() {
    const auto& trains = systemManager.getAllTrains();
    allTrainsTable->setRowCount(0);
    for (const auto& pair : trains) {
        const Train& t = pair.second;
        int row = allTrainsTable->rowCount();
        allTrainsTable->insertRow(row);
        
        allTrainsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(t.getId())));
        allTrainsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(t.getType())));
        allTrainsTable->setItem(row, 2, new QTableWidgetItem(QString::number(t.getTotalSeats())));
        
        QString routeStr;
        const auto& route = t.getRoute();
        for (size_t i = 0; i < route.size(); ++i) {
            routeStr += QString::fromStdString(route[i].stationName);
            if (i < route.size() - 1) {
                routeStr += "->";
            }
        }
        allTrainsTable->setItem(row, 3, new QTableWidgetItem(routeStr));
    }
}

void MainWindow::updatePassengerView() {
    refreshOrderTable();
}

void MainWindow::updateAdminView() {
    refreshTrainTable();
}
