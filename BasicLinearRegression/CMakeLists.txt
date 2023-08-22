 cmake_minimum_required(VERSION 3.6)
 project(LINEAR_REGRESSION)

 set(CMAKE_CXX_STANDART 17)


 set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
 set(INC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
 set(SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)


 set(SOURCES 
   ${SRC_DIR}/csv.cpp
   ${SRC_DIR}/linearRegression.cpp
   ${SRC_DIR}/main.cpp
 )

 include_directories(${INC_DIR})

 add_executable(out ${SOURCES})
