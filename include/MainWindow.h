/**
 * @file MainWindow.h
 * @brief Definition of the main GUI window.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include "SystemManager.h"

/**
 * @class MainWindow
 * @brief Main application window.
 * 
 * Manages the UI using a QStackedWidget to switch between
 * Login, Register, Passenger, and Admin views.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Auth Slots
    void handleLogin();
    void handleRegister();
    void showRegisterPage();
    void showLoginPage();
    void handleLogout();

    // Passenger Slots
    void handleSearch();
    void refreshOrderTable();

    // Admin Slots
    void handleAddTrain();
    void refreshTrainTable();

private:
    SystemManager systemManager; ///< Backend controller
    QStackedWidget *stackedWidget; ///< Stacked widget for page navigation

    // Login Page Widgets
    QLineEdit *loginUsernameInput;
    QLineEdit *loginPasswordInput;

    // Register Page Widgets
    QLineEdit *regUsernameInput;
    QLineEdit *regPasswordInput;
    QLineEdit *regNameInput;
    QLineEdit *regIdInput;

    // Passenger Dashboard Widgets
    QWidget *passengerPage;
    QLineEdit *searchStartInput;
    QLineEdit *searchEndInput;
    QDateEdit *searchDateInput;
    QTableWidget *trainResultTable;
    QTableWidget *orderHistoryTable;
    
    // Admin Dashboard Widgets
    QWidget *adminPage;
    QTableWidget *allTrainsTable;
    // Add Train Inputs
    QLineEdit *addTrainIdInput;
    QLineEdit *addTrainTypeInput;
    QLineEdit *addTrainSeatsInput;
    QComboBox *addTrainRouteMode;
    QLineEdit *addTrainCustomRouteInput;

    /**
     * @brief Initializes the UI components.
     */
    void setupUi();
    
    void createLoginPage();
    void createRegisterPage();
    void createPassengerPage();
    void createAdminPage();
    
    void updatePassengerView();
    void updateAdminView();
};

#endif // MAINWINDOW_H
