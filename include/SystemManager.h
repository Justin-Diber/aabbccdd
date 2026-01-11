/**
 * @file SystemManager.h
 * @brief Definition of SystemManager class.
 * 
 * This class acts as the central controller for the system.
 * It manages users, trains, and handles business logic like booking and refunds.
 */

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include "User.h"
#include "Train.h"
#include "Order.h"

class SystemManager {
private:
    map<string, shared_ptr<User>> users; ///< Map of username to User object
    map<string, Train> trains;           ///< Map of trainId to Train object
    shared_ptr<User> currentUser;        ///< Pointer to the currently logged-in user

    // Helper functions for persistence (placeholders)
    void loadData();
    void saveData();

public:
    SystemManager();
    ~SystemManager();

    // User Management
    
    /**
     * @brief Registers a new user.
     * @return true if successful, false if username exists.
     */
    bool registerUser(const string& username, const string& password, const string& name, const string& id);
    
    /**
     * @brief Authenticates a user.
     * @return Pointer to User if successful, nullptr otherwise.
     */
    shared_ptr<User> login(const string& username, const string& password);
    
    /**
     * @brief Logs out the current user.
     */
    void logout();
    
    /**
     * @brief Gets the current logged-in user.
     */
    shared_ptr<User> getCurrentUser() const { return currentUser; }

    // Train Management (Admin)
    
    /**
     * @brief Adds a new train to the system.
     */
    void addTrain(const Train& train);
    
    /**
     * @brief Removes a train from the system.
     */
    bool deleteTrain(const string& trainId);
    
    /**
     * @brief Retrieves a train by ID.
     */
    Train* getTrain(const string& trainId);
    
    /**
     * @brief Searches for trains between two stations on a given date.
     * @return Vector of trains that have availability.
     */
    vector<Train> searchTrains(const string& startStation, const string& endStation, const string& date);
    
    /**
     * @brief Returns all trains (for admin view).
     */
    const map<string, Train>& getAllTrains() const { return trains; }

    // Order Management
    
    /**
     * @brief Books a ticket for the current user.
     * @return true if successful.
     */
    bool bookTicket(const string& trainId, const string& start, const string& end, const string& date, int count = 1);
    
    /**
     * @brief Refunds a ticket for the current user.
     * @return true if successful.
     */
    bool refundTicket(const string& orderId);

    /**
     * @brief Initializes test data for demonstration.
     */
    void initTestData();
};

#endif // SYSTEMMANAGER_H
