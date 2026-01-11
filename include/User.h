/**
 * @file User.h
 * @brief Definition of User class hierarchy.
 */

#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <vector>
#include "Order.h"

using namespace std;

/**
 * @class User
 * @brief Abstract Base Class for users.
 * 
 * Provides common attributes like username, password, real name, and ID card.
 * Defines pure virtual functions for polymorphism.
 */
class User {
protected:
    string username;
    string password;
    string realName;
    string idCard;

public:
    User(string u, string p, string name, string id) 
        : username(u), password(p), realName(name), idCard(id) {}
    virtual ~User() {}

    string getUsername() const { return username; }
    string getRealName() const { return realName; }
    
    /**
     * @brief Verifies password.
     * @param inputPwd Password to check.
     * @return true if matches.
     */
    bool checkPassword(const string& inputPwd) const {
        return password == inputPwd;
    }

    // Pure virtual function - Polymorphism
    virtual string getRole() const = 0;
    virtual void displayMenu() const = 0;
};

/**
 * @class Passenger
 * @brief Represents a normal passenger user.
 * Inherits from User. Manages personal order history.
 */
class Passenger : public User {
private:
    vector<Order> orderHistory;

public:
    Passenger(string u, string p, string name, string id) 
        : User(u, p, name, id) {}

    string getRole() const override { return "Passenger"; }
    void displayMenu() const override;

    void addOrder(const Order& order);
    void cancelOrder(const string& orderId);
    vector<Order>& getOrders() { return orderHistory; }
};

/**
 * @class Admin
 * @brief Represents an administrator user.
 * Inherits from User. Has privileges to manage trains.
 */
class Admin : public User {
public:
    Admin(string u, string p, string name, string id) 
        : User(u, p, name, id) {}

    string getRole() const override { return "Admin"; }
    void displayMenu() const override;
};

#endif // USER_H
