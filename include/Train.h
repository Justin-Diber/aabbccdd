/**
 * @file Train.h
 * @brief Definition of Train class and Stop structure.
 * 
 * This file contains the data structures representing a train and its route information.
 * It handles ticket availability, pricing, and schedule management.
 */

#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

/**
 * @brief Represents a single stop on a train's route.
 */
struct Stop {
    string stationName;   ///< Name of the station
    string arrivalTime;   ///< Arrival time in HH:MM format
    string departureTime; ///< Departure time in HH:MM format
    double priceFromStart; ///< Cumulative price from the starting station
    int distance;         ///< Distance from the starting station in km
};

/**
 * @class Train
 * @brief Represents a train in the system.
 * 
 * The Train class manages the train's basic information (ID, type, capacity),
 * its route (sequence of stops), and the seat inventory for different dates.
 * It provides methods to check ticket availability and book/release tickets.
 */
class Train {
private:
    string trainId;      ///< Unique identifier for the train (e.g., G101)
    string type;         ///< Type of the train (e.g., High-Speed, Normal)
    int totalSeats;      ///< Total number of seats available per segment
    vector<Stop> route;  ///< Ordered list of stops
    
    /**
     * @brief Seat inventory management.
     * Map Key: Date string (YYYY-MM-DD)
     * Map Value: Vector of integers representing available seats for each segment.
     * Segment i corresponds to the path between stop[i] and stop[i+1].
     */
    map<string, vector<int>> seatInventory; 

public:
    /**
     * @brief Default constructor.
     */
    Train() = default;

    /**
     * @brief Parameterized constructor.
     * @param id Train ID
     * @param t Train Type
     * @param seats Total capacity
     */
    Train(string id, string t, int seats);

    // Getters
    string getId() const { return trainId; }
    string getType() const { return type; }
    int getTotalSeats() const { return totalSeats; }
    const vector<Stop>& getRoute() const { return route; }

    /**
     * @brief Adds a stop to the train's route.
     * @param stop The Stop object to add.
     */
    void addStop(const Stop& stop);
    
    /**
     * @brief Checks if there are enough tickets for a given segment.
     * @param date Travel date
     * @param startStation Name of starting station
     * @param endStation Name of destination station
     * @param count Number of tickets needed
     * @return true if tickets are available, false otherwise
     */
    bool hasTickets(const string& date, const string& startStation, const string& endStation, int count = 1);
    
    /**
     * @brief Books tickets for a given segment.
     * Decreases the seat inventory for all segments between start and end.
     * @param date Travel date
     * @param startStation Name of starting station
     * @param endStation Name of destination station
     * @param count Number of tickets to book
     * @return true if booking successful, false if not enough tickets
     */
    bool bookTickets(const string& date, const string& startStation, const string& endStation, int count = 1);
    
    /**
     * @brief Releases tickets (used for refunds).
     * Increases the seat inventory for all segments between start and end.
     * @param date Travel date
     * @param startStation Name of starting station
     * @param endStation Name of destination station
     * @param count Number of tickets to release
     */
    void releaseTickets(const string& date, const string& startStation, const string& endStation, int count = 1);

    /**
     * @brief Calculates the price between two stations.
     * @param startStation Name of starting station
     * @param endStation Name of destination station
     * @return The price difference between the two stations
     */
    double getPrice(const string& startStation, const string& endStation);

    /**
     * @brief Gets the departure time for a specific station.
     * @param station Station name
     * @return Time string (HH:MM)
     */
    string getDepartureTime(const string& station);

    /**
     * @brief Gets the arrival time for a specific station.
     * @param station Station name
     * @return Time string (HH:MM)
     */
    string getArrivalTime(const string& station);

    /**
     * @brief Overloaded output stream operator for printing Train info.
     */
    friend ostream& operator<<(ostream& os, const Train& train);
};

#endif // TRAIN_H
