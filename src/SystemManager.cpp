/**
 * @file SystemManager.cpp
 * @brief Implementation of SystemManager logic.
 */

#include "SystemManager.h"
#include <iostream>

SystemManager::SystemManager() {
    initTestData();
}

SystemManager::~SystemManager() {
    // Save data if needed
}

/**
 * @brief Initializes the system with dummy data.
 * Creates an admin, a user, and some trains.
 */
void SystemManager::initTestData() {
    // Add Admin
    users["admin"] = make_shared<Admin>("admin", "admin123", "System Admin", "000000");
    
    // Add Passenger
    users["user1"] = make_shared<Passenger>("user1", "123456", "John Doe", "123456789012345678");

    // Add Train G101: Beijing -> Shanghai
    Train t1("G101", "High-Speed", 100);
    t1.addStop({"Beijing", "08:00", "08:00", 0.0, 0});
    t1.addStop({"Jinan", "09:30", "09:35", 150.0, 400});
    t1.addStop({"Nanjing", "11:30", "11:35", 350.0, 1000});
    t1.addStop({"Shanghai", "13:00", "13:00", 553.0, 1318});
    addTrain(t1);

    // Add Train K505: Beijing -> Xi'an
    Train t2("K505", "Normal", 200);
    t2.addStop({"Beijing", "07:00", "07:00", 0.0, 0});
    t2.addStop({"Shijiazhuang", "10:00", "10:15", 50.0, 300});
    t2.addStop({"Zhengzhou", "13:00", "13:20", 120.0, 700});
    t2.addStop({"Xi'an", "18:00", "18:00", 200.0, 1200});
    addTrain(t2);
}

/**
 * @brief Registers a new user.
 * Checks for duplicate usernames.
 */
bool SystemManager::registerUser(const string& username, const string& password, const string& name, const string& id) {
    if (users.find(username) != users.end()) {
        return false;
    }
    users[username] = make_shared<Passenger>(username, password, name, id);
    return true;
}

/**
 * @brief Logs in a user.
 * Verifies credentials and sets current user.
 */
shared_ptr<User> SystemManager::login(const string& username, const string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second->checkPassword(password)) {
        currentUser = it->second;
        return currentUser;
    }
    return nullptr;
}

void SystemManager::logout() {
    currentUser = nullptr;
}

void SystemManager::addTrain(const Train& train) {
    trains[train.getId()] = train;
}

bool SystemManager::deleteTrain(const string& trainId) {
    auto it = trains.find(trainId);
    if (it != trains.end()) {
        trains.erase(it);
        return true;
    }
    return false;
}

Train* SystemManager::getTrain(const string& trainId) {
    auto it = trains.find(trainId);
    if (it != trains.end()) {
        return &(it->second);
    }
    return nullptr;
}

/**
 * @brief Searches for trains.
 * Returns a list of trains that have availability between start and end stations.
 */
vector<Train> SystemManager::searchTrains(const string& startStation, const string& endStation, const string& date) {
    vector<Train> result;
    for (auto& pair : trains) {
        Train& t = pair.second;
        if (t.hasTickets(date, startStation, endStation)) {
            result.push_back(t);
        }
    }
    return result;
}

/**
 * @brief Books a ticket.
 * Reduces inventory and creates an order for the current user.
 */
bool SystemManager::bookTicket(const string& trainId, const string& start, const string& end, const string& date, int count) {
    if (!currentUser) return false;
    
    Train* t = getTrain(trainId);
    if (!t) return false;

    if (t->bookTickets(date, start, end, count)) {
        double price = t->getPrice(start, end) * count;
        string time = t->getDepartureTime(start);
        Order order(currentUser->getUsername(), trainId, start, end, date, time, price, count);
        
        // If current user is passenger, add to history
        Passenger* p = dynamic_cast<Passenger*>(currentUser.get());
        if (p) {
            p->addOrder(order);
        }
        return true;
    }
    return false;
}

/**
 * @brief Refunds a ticket.
 * Cancels the order and releases the inventory.
 */
bool SystemManager::refundTicket(const string& orderId) {
    if (!currentUser) return false;

    Passenger* p = dynamic_cast<Passenger*>(currentUser.get());
    if (!p) return false;

    vector<Order>& orders = p->getOrders();
    for (auto& order : orders) {
        if (order.getOrderId() == orderId && order.getStatus() == PAID) {
            order.setStatus(CANCELLED);
            Train* t = getTrain(order.getTrainId());
            if (t) {
                t->releaseTickets(order.getDate(), order.getStartStation(), order.getEndStation(), order.getTicketCount());
            }
            return true;
        }
    }
    return false;
}
