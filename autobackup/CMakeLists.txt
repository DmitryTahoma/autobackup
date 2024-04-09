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
    "Include/MyLZMA.h"
)

set(SOURCE_FILES
    "Source/CConfiguration.cpp"
    "Source/DumperBase.cpp"
    "Source/MySQLDumper.cpp"
)

find_package(LibLZMA REQUIRED)
add_executable (autobackup "main.cpp" ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(autobackup LibLZMA::LibLZMA)
# target_include_directories(autobackup PRIVATE ${LibLZMA_INCLUDE_DIRS})