cmake_minimum_required(VERSION 3.17)  
  
project(LyTest)  
  
set(CMAKE_CXX_STANDARD 14)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
  
add_executable(lytest MSB.cpp)  
set_target_properties(lytest PROPERTIES OUTPUT_NAME "lytest")  
  
if(CMAKE_COMPILER_IS_GNUCXX)  
    set_target_properties(lytest PROPERTIES COMPILE_FLAGS "-O2")  
endif()