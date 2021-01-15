cmake_minimum_required(VERSION 3.0.0)
project(ex6 VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include(CTest)
enable_testing()


add_executable(ex6 MainTrain.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp Server.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
