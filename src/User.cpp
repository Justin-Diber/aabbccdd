/**
 * @file User.cpp
 * @brief Implementation of User subclasses.
 */

#include "User.h"

/**
 * @brief Displays the passenger menu.
 */
void Passenger::displayMenu() const {
    cout << "=== Passenger Menu (" << username << ") ===" << endl;
    cout << "1. Search Tickets" << endl;
    cout << "2. My Orders" << endl;
    cout << "3. Profile" << endl;
    cout << "4. Logout" << endl;
}

/**
 * @brief Adds an order to the history.
 */
void Passenger::addOrder(const Order& order) {
    orderHistory.push_back(order);
}

/**
 * @brief Marks an order as cancelled.
 */
void Passenger::cancelOrder(const string& orderId) {
    for (auto& order : orderHistory) {
        if (order.getOrderId() == orderId) {
            order.setStatus(CANCELLED);
            return;
        }
    }
}

/**
 * @brief Displays the admin menu.
 */
void Admin::displayMenu() const {
    cout << "=== Admin Menu (" << username << ") ===" << endl;
    cout << "1. Manage Trains (Add/Remove)" << endl;
    cout << "2. Manage Users" << endl;
    cout << "3. System Status" << endl;
    cout << "4. Logout" << endl;
}
