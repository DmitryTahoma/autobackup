# CMakeList.txt : CMake project for autobackup, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (autobackup "autobackup.cpp" "autobackup.h" "SDatabaseConfig.h" "EDatabaseProvider.h" "CConfiguration.h" "CConfiguration.cpp")

# TODO: Add tests and install targets if needed.
