cmake_minimum_required(VERSION 3.5.1)
project(phpLogGuard)
set(CMAKE_CXX_STANDARD 11)

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -pthread")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -pthread")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

#引入头文件
INCLUDE_DIRECTORIES(include)

set(SOURCE_FILES main.cpp include/NodeManager.h src/core/node/NodeManager.cpp include/UnixCommand.h src/core/os/UnixCommand.cpp include/IniConfig.h src/core/config/IniConfig.cpp)
#加入主要文件
add_executable(phpLogGuard ${SOURCE_FILES})
#CFLAGS='-g -O0'
#进行动态库连接

