# aabbccdd Simulation System

This is a C++ course project implementing a simplified version of the aabbccdd system.
It features a Graphical User Interface (GUI) built with Qt 6 and a robust backend logic handling users, trains, and orders.

## Features

1.  **User Management**
    *   Registration (Passenger)
    *   Login (Passenger & Admin)
    *   Logout
2.  **Train Management (Admin)**
    *   View all trains
    *   Add new trains (Simplified)
    *   (Delete trains - Backend supported)
3.  **Passenger Services**
    *   Search trains by Start/End Station and Date
    *   View ticket prices and availability
    *   Book tickets
    *   View Order History
    *   Refund/Cancel tickets
4.  **System**
    *   Data persistence (Memory-based for this demo, easily extensible to file/DB)
    *   OOP Design (Inheritance, Polymorphism, Encapsulation)

## Requirements

*   C++17 Compiler (GCC, Clang, MSVC)
*   CMake 3.16+
*   Qt 6 (Core, Gui, Widgets)

## Project Structure

*   `include/`: Header files (.h)
*   `src/`: Source files (.cpp)
*   `CMakeLists.txt`: Build configuration
*   `README.md`: This file

## How to Build

### 0. Install Dependencies

#### macOS (Recommended)
The easiest way is using [Homebrew](https://brew.sh/):

```bash
brew install qt
# Link qt for cmake to find it easily (optional but recommended)
brew link qt
```

If CMake cannot find Qt6, you may need to export the path:
```bash
export CMAKE_PREFIX_PATH=$(brew --prefix qt)
```

#### Linux (Ubuntu/Debian)

1.  **Install Build Tools & CMake**:
    ```bash
    sudo apt update
    sudo apt install build-essential cmake
    ```

2.  **Install Qt6**:
    ```bash
    sudo apt install qt6-base-dev qt6-base-dev-tools libgl1-mesa-dev
    ```
    *Note: If your distribution's repository doesn't have Qt6, you may need to use the official [Qt Online Installer](https://www.qt.io/download-qt-installer).*

3.  **Build & Run**:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ./RailwayTicketSystem
    ```

#### Windows / Linux
Download the official **Qt Online Installer** from [qt.io](https://www.qt.io/download-qt-installer).
Select **Qt 6.x** and the component for your compiler (MinGW or MSVC for Windows, GCC for Linux).

### 1. Build with Qt (GUI)

Ensure you have Qt 6 installed and found by CMake.

```bash
mkdir build
cd build
cmake ..
make
./RailwayTicketSystem
```

### 2. Build Test Runner (No Qt required)

If you don't have Qt installed or want to verify the logic quickly, you can build the console-based test runner.

```bash
mkdir build
cd build
cmake ..
make TestRunner
./TestRunner
```

## Testing

The project includes a `src/test_main.cpp` file that acts as a startup test example. It verifies:
1.  User Registration & Login
2.  Train Search Logic
3.  Booking Flow
4.  Inventory Management
5.  Refund Flow

Run `./TestRunner` to see the results.

## Design Highlights

*   **Classes**: `User` (Base), `Passenger`, `Admin` (Derived), `Train`, `Order`, `SystemManager`.
*   **Polymorphism**: `User::displayMenu()` allows different behaviors for Admins and Passengers.
*   **Operator Overloading**: `<<` operator overloaded for `Train` and `Order` for easy debugging/printing.
*   **Encapsulation**: All data members are private with appropriate getters/setters.
*   **No Global Variables**: All state is managed within `SystemManager` and `MainWindow`.

## Author

aabbccdd
