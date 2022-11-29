cmake_minimum_required(VERSION 3.0.0)
project(TerminalBA VERSION 0.1.0)

set(CMAKE_CXX_COMPILER "C:/ProgramData/chocolatey/bin/g++.exe")

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(TerminalBA main.cpp)

target_link_libraries( TerminalBA ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
