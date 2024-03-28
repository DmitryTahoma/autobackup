# CMakeList.txt : CMake project for autobackup, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(HEADER_FILES
    "Include/CConfiguration.h"
    "Include/DumperBase.h"
    "Include/EDatabaseProvider.h"
    "Include/MySQLDumper.h"
    "Include/SDatabaseConfig.h"
)

set(SOURCE_FILES
    "Source/CConfiguration.cpp"
    "Source/DumperBase.cpp"
    "Source/MySQLDumper.cpp"
)

add_executable (autobackup "main.cpp" ${HEADER_FILES} ${SOURCE_FILES})