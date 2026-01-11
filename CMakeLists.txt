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
# Try finding Qt6 first, then fallback to Qt5
find_package(Qt6 COMPONENTS Widgets Core Gui QUIET)

if(Qt6_FOUND)
    message(STATUS "Qt6 found, building with Qt6.")
    set(QT_LIBRARIES Qt6::Widgets Qt6::Core Qt6::Gui)
else()
    message(STATUS "Qt6 not found, trying Qt5...")
    find_package(Qt5 COMPONENTS Widgets Core Gui QUIET)
    if(Qt5_FOUND)
        message(STATUS "Qt5 found, building with Qt5.")
        set(QT_LIBRARIES Qt5::Widgets Qt5::Core Qt5::Gui)
    endif()
endif()

if(Qt6_FOUND OR Qt5_FOUND)
    add_executable(RailwayTicketSystem 
        src/main.cpp 
        src/MainWindow.cpp 
        ${CORE_SOURCES}
        ${HEADERS}
    )
    target_link_libraries(RailwayTicketSystem PRIVATE ${QT_LIBRARIES})
else()
    message(WARNING "Neither Qt6 nor Qt5 found. Skipping GUI application build. Please install Qt.")
endif()

# Test Runner (Console)
add_executable(TestRunner src/test_main.cpp ${CORE_SOURCES})
