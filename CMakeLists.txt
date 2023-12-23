cmake_minimum_required(VERSION 3.17)    
  
project(LyTest)    
  
set(CMAKE_CXX_STANDARD 11)    
set(CMAKE_CXX_STANDARD_REQUIRED ON)    
  
# 获取源代码的目录  
get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)  
  
add_executable(lytest MSB.cpp)    
set_target_properties(lytest PROPERTIES OUTPUT_NAME "lytest")   
  
# 设置输出路径为源代码目录  
set_target_properties(lytest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PARENT_DIR}")  
  
if(CMAKE_COMPILER_IS_GNUCXX)    
    set_target_properties(lytest PROPERTIES COMPILE_FLAGS "-O2")    
endif()