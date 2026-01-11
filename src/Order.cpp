/**
 * @file Order.cpp
 * @brief Implementation of Order class.
 */

#include "Order.h"
#include <sstream>
#include <iomanip>

/**
 * @brief Constructor.
 * Initializes order details and generates a unique ID.
 */
Order::Order(string uName, string tId, string start, string end, string d, string time, double p, int count)
    : username(uName), trainId(tId), startStation(start), endStation(end), 
      date(d), departureTime(time), price(p), ticketCount(count), status(PAID) {
    orderId = generateOrderId();
    createTime = std::time(nullptr);
}

/**
 * @brief Generates a unique order ID.
 * Format: YYYYMMDDHHMMSS + 4-digit counter.
 * @return String ID.
 */
string Order::generateOrderId() {
    static int counter = 0;
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << (1900 + ltm->tm_year) 
       << setfill('0') << setw(2) << (1 + ltm->tm_mon)
       << setw(2) << ltm->tm_mday
       << setw(2) << ltm->tm_hour
       << setw(2) << ltm->tm_min
       << setw(2) << ltm->tm_sec
       << setw(4) << (++counter);
    
    return ss.str();
}

/**
 * @brief Output stream operator.
 * Prints order details.
 */
ostream& operator<<(ostream& os, const Order& order) {
    os << "Order ID: " << order.orderId << "\n"
       << "Train: " << order.trainId << "\n"
       << "Route: " << order.startStation << " -> " << order.endStation << "\n"
       << "Time: " << order.date << " " << order.departureTime << "\n"
       << "Tickets: " << order.ticketCount << "\n"
       << "Price: " << order.price << "\n"
       << "Status: " << (order.status == PAID ? "Paid" : (order.status == CANCELLED ? "Cancelled" : "Completed")) << endl;
    return os;
}
