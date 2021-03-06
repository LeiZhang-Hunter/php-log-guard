cmake_minimum_required(VERSION 3.4.19)
project(php-atel-agent)
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Release")

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Add -O0 to remove optimizations when using gcc
IF (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")

#引入头文件
INCLUDE_DIRECTORIES(include)

set(SOURCE_FILES
        main.cpp
        include/NodeManager.h
        src/core/node/NodeManager.cpp
        include/os/UnixCommand.h
        src/core/os/UnixCommand.cpp
        include/config/IniConfig.h
        src/core/config/IniConfig.cpp
        include/lock/FilePersistenceLock.h
        src/core/lock/FilePersistenceLock.cpp
        include/Noncopyable.h
        src/core/event/EventLoop.cpp
        include/event/EventLoop.h
        include/event/Channel.h
        src/core/event/Channel.cpp
        src/core/os/UnixThread.cpp
        include/os/UnixThread.h
        src/core/os/UnixSignalDescription.cpp
        include/os/UnixSignalDescription.h
        src/core/event/EventWakeup.cpp
        include/event/EventWakeup.h
        include/os/UnixThread.h
        include/os/UnixInodeWatcher.h
        src/core/os/UnixInodeWatcher.cpp
        src/core/event/FileEvent.cpp
        include/event/FileEvent.h
        src/app/PHPError.cpp
        include/app/PHPError.h
        src/core/os/UnixCountDownLatch.cpp
        include/os/UnixCountDownLatch.h
        include/os/UnixMutex.h
        include/os/UnixConditionMutex.h
        include/os/UnixAutoMutex.h
        include/os/UnixThreadProc.h
        src/core/os/UnixThreadProc.cpp src/app/AtelFormat.cpp
        include/app/AtelFormat.h
        src/core/os/UnixUtil.cpp
        include/os/UnixUtil.h
        src/core/os/UnixTimer.cpp
        include/os/UnixTimer.h)
#加入主要文件
add_executable(php-atel-agent ${SOURCE_FILES})
#CFLAGS='-g -O0'
#进行动态库连接

