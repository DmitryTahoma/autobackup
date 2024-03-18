# CMakeList.txt : CMake project for autobackup, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (autobackup "main.cpp"  "Include/SDatabaseConfig.h" "Include/EDatabaseProvider.h" "Include/CConfiguration.h" "Source/CConfiguration.cpp")

# TODO: Add tests and install targets if needed.
