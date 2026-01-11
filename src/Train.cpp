/**
 * @file Train.cpp
 * @brief Implementation of the Train class.
 */

#include "Train.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Constructor for Train.
 * Initializes the train with basic details.
 */
Train::Train(string id, string t, int seats) 
    : trainId(id), type(t), totalSeats(seats) {}

/**
 * @brief Adds a stop to the route.
 * @param stop The Stop structure containing station details.
 */
void Train::addStop(const Stop& stop) {
    route.push_back(stop);
}

/**
 * @brief Helper function to find the index of a station in the route.
 * @param route The vector of stops.
 * @param stationName The name of the station to find.
 * @return The index of the station, or -1 if not found.
 */
int getStationIndex(const vector<Stop>& route, const string& stationName) {
    for (size_t i = 0; i < route.size(); ++i) {
        if (route[i].stationName == stationName) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Checks ticket availability.
 * Verifies if there are enough seats in all segments between start and end stations.
 */
bool Train::hasTickets(const string& date, const string& startStation, const string& endStation, int count) {
    int startIndex = getStationIndex(route, startStation);
    int endIndex = getStationIndex(route, endStation);

    // Validate stations order
    if (startIndex == -1 || endIndex == -1 || startIndex >= endIndex) {
        return false;
    }

    // Initialize inventory for this date if not exists
    if (seatInventory.find(date) == seatInventory.end()) {
        // By default, all segments have totalSeats
        // Number of segments = route.size() - 1
        seatInventory[date] = vector<int>(route.size() - 1, totalSeats);
    }

    const vector<int>& dailySeats = seatInventory[date];

    // Check every segment from startIndex to endIndex-1
    for (int i = startIndex; i < endIndex; ++i) {
        if (dailySeats[i] < count) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Books tickets.
 * Decrements the available seat count for the specified segments.
 */
bool Train::bookTickets(const string& date, const string& startStation, const string& endStation, int count) {
    if (!hasTickets(date, startStation, endStation, count)) {
        return false;
    }

    int startIndex = getStationIndex(route, startStation);
    int endIndex = getStationIndex(route, endStation);

    // We know inventory exists because hasTickets returned true (and it initializes if missing)
    vector<int>& dailySeats = seatInventory[date];
    for (int i = startIndex; i < endIndex; ++i) {
        dailySeats[i] -= count;
    }
    return true;
}

/**
 * @brief Releases tickets.
 * Increments the available seat count for the specified segments.
 * Used for cancellations/refunds.
 */
void Train::releaseTickets(const string& date, const string& startStation, const string& endStation, int count) {
    int startIndex = getStationIndex(route, startStation);
    int endIndex = getStationIndex(route, endStation);
    
    // If date not in inventory, nothing to release (should not happen if order exists)
    if (seatInventory.find(date) == seatInventory.end()) return;

    vector<int>& dailySeats = seatInventory[date];
    for (int i = startIndex; i < endIndex; ++i) {
        dailySeats[i] += count;
        // Cap at total seats
        if (dailySeats[i] > totalSeats) dailySeats[i] = totalSeats;
    }
}

/**
 * @brief Calculates ticket price.
 * Price is determined by the difference in cumulative price between end and start stations.
 */
double Train::getPrice(const string& startStation, const string& endStation) {
    int startIndex = getStationIndex(route, startStation);
    int endIndex = getStationIndex(route, endStation);
    
    if (startIndex == -1 || endIndex == -1 || startIndex >= endIndex) return 0.0;

    return route[endIndex].priceFromStart - route[startIndex].priceFromStart;
}

/**
 * @brief Gets departure time.
 */
string Train::getDepartureTime(const string& station) {
    int idx = getStationIndex(route, station);
    if (idx != -1) return route[idx].departureTime;
    return "";
}

/**
 * @brief Gets arrival time.
 */
string Train::getArrivalTime(const string& station) {
    int idx = getStationIndex(route, station);
    if (idx != -1) return route[idx].arrivalTime;
    return "";
}

/**
 * @brief Output stream operator.
 * Displays train ID, type, and full route.
 */
ostream& operator<<(ostream& os, const Train& train) {
    os << "Train: " << train.trainId << " (" << train.type << ")" << endl;
    os << "Route: ";
    for (const auto& stop : train.route) {
        os << stop.stationName << " -> ";
    }
    os << "END";
    return os;
}
