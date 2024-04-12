# CMakeList.txt : CMake project for autobackup, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(HEADER_FILES
    "Include/CConfiguration.h"
    "Include/CDumperBase.h"
    "Include/EDatabaseProvider.h"
    "Include/CMySQLDumper.h"
    "Include/SDatabaseConfig.h"
    "Include/CLZMAUtil.h"
    "Include/CDateTime.h"
    "Include/CTimeSpan.h"
)

set(SOURCE_FILES
    "Source/CConfiguration.cpp"
    "Source/CDumperBase.cpp"
    "Source/CMySQLDumper.cpp"
    "Source/CLZMAUtil.cpp"
    "Source/CDateTime.cpp"
    "Source/CTimeSpan.cpp"
)

find_package(LibLZMA REQUIRED)
add_executable (autobackup "main.cpp" ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(autobackup LibLZMA::LibLZMA)
# target_include_directories(autobackup PRIVATE ${LibLZMA_INCLUDE_DIRS})