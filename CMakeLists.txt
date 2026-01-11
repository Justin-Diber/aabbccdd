cmake_minimum_required(VERSION 3.16)

project(RailwayTicketSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

# Find all header files
file(GLOB HEADERS "include/*.h")

# Core Source Files
set(CORE_SOURCES
    src/User.cpp
    src/Train.cpp
    src/Order.cpp
    src/SystemManager.cpp
)

# GUI Application
find_package(Qt6 COMPONENTS Widgets Core Gui QUIET)

if(Qt6_FOUND)
    message(STATUS "Qt6 found, building GUI application.")
    add_executable(RailwayTicketSystem 
        src/main.cpp 
        src/MainWindow.cpp 
        ${CORE_SOURCES}
        ${HEADERS}
    )
    target_link_libraries(RailwayTicketSystem PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
else()
    message(WARNING "Qt6 not found. Skipping GUI application build. Please install Qt6 to build the GUI.")
endif()

# Test Runner (Console)
add_executable(TestRunner src/test_main.cpp ${CORE_SOURCES})
