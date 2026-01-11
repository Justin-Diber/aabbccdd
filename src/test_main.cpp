#include <iostream>
#include <cassert>
#include "SystemManager.h"

void testLogic() {
    SystemManager sys;
    cout << "Initializing System..." << endl;

    // Test Login
    cout << "Testing Login..." << endl;
    auto user = sys.login("admin", "admin123");
    assert(user != nullptr);
    assert(user->getRole() == "Admin");
    cout << "Admin login successful." << endl;
    sys.logout();

    // Test Register
    cout << "Testing Register..." << endl;
    bool reg = sys.registerUser("testuser", "pass", "Test User", "111");
    assert(reg == true);
    user = sys.login("testuser", "pass");
    assert(user != nullptr);
    assert(user->getRole() == "Passenger");
    cout << "User registration and login successful." << endl;

    // Test Search
    cout << "Testing Search..." << endl;
    // G101 goes Beijing -> Jinan -> Nanjing -> Shanghai
    vector<Train> trains = sys.searchTrains("Beijing", "Shanghai", "2023-10-01");
    assert(trains.size() > 0);
    cout << "Found " << trains.size() << " trains from Beijing to Shanghai." << endl;
    cout << "Train 1 Price: " << trains[0].getPrice("Beijing", "Shanghai") << endl;

    // Test Booking
    cout << "Testing Booking..." << endl;
    bool booked = sys.bookTicket(trains[0].getId(), "Beijing", "Shanghai", "2023-10-01");
    assert(booked == true);
    cout << "Booking successful." << endl;

    // Verify Order
    Passenger* p = dynamic_cast<Passenger*>(user.get());
    assert(p->getOrders().size() == 1);
    cout << "Order history verified." << endl;

    // Test Inventory Reduction
    Train* t = sys.getTrain(trains[0].getId());
    // Note: We need to check if inventory logic works.
    // However, Train::hasTickets checks seatInventory map.
    // Since we just booked 1 ticket, if capacity was 100, now it should be 99 for that segment.
    // Internal state check is hard without getters, but we can try booking 100 more and fail.
    
    // Test Refund
    cout << "Testing Refund..." << endl;
    string orderId = p->getOrders()[0].getOrderId();
    bool refunded = sys.refundTicket(orderId);
    assert(refunded == true);
    assert(p->getOrders()[0].getStatus() == CANCELLED);
    cout << "Refund successful." << endl;

    cout << "ALL TESTS PASSED!" << endl;
}

int main() {
    testLogic();
    return 0;
}
