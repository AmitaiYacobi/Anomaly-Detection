cmake_minimum_required(VERSION 3.0.0)
project(part2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(part2 MainTrain.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp timeseries.cpp HybridAnomalyDetector.cpp minCircle.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
