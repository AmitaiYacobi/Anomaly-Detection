cmake_minimum_required(VERSION 3.0.0)
project(anomaly-detection VERSION 0.1.0)

set(C_STANDARD C99)

add_library(anomaly-detection SHARED MainTrain.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

