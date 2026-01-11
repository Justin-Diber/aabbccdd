/**
 * @file Order.h
 * @brief Definition of Order class and OrderStatus enum.
 */

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <ctime>

using namespace std;

/**
 * @enum OrderStatus
 * @brief Status of an order.
 */
enum OrderStatus {
    PAID,       ///< Order is paid and valid
    CANCELLED,  ///< Order has been cancelled/refunded
    COMPLETED   ///< Trip is completed
};

/**
 * @class Order
 * @brief Represents a ticket order in the system.
 */
class Order {
private:
    string orderId;       ///< Unique Order ID
    string username;      ///< User who placed the order
    string trainId;       ///< Train ID
    string startStation;  ///< Departure Station
    string endStation;    ///< Arrival Station
    string date;          ///< Travel Date
    string departureTime; ///< Departure Time
    double price;         ///< Total Price
    int ticketCount;      ///< Number of tickets
    OrderStatus status;   ///< Current status
    time_t createTime;    ///< Order creation timestamp

public:
    Order() = default;

    /**
     * @brief Parameterized Constructor.
     */
    Order(string uName, string tId, string start, string end, string d, string time, double p, int count);

    // Getters
    string getOrderId() const { return orderId; }
    string getUsername() const { return username; }
    string getTrainId() const { return trainId; }
    string getStartStation() const { return startStation; }
    string getEndStation() const { return endStation; }
    string getDate() const { return date; }
    double getPrice() const { return price; }
    OrderStatus getStatus() const { return status; }
    int getTicketCount() const { return ticketCount; }

    // Setters
    void setStatus(OrderStatus s) { status = s; }

    /**
     * @brief Generates a unique order ID based on time and counter.
     * @return Unique ID string
     */
    static string generateOrderId();

    /**
     * @brief Overloaded output operator.
     */
    friend ostream& operator<<(ostream& os, const Order& order);
};

#endif // ORDER_H
