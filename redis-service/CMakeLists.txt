cmake_minimum_required(VERSION 3.25)
set(PROJECT_NAME redis-service)
project(${PROJECT_NAME})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(${PROJECT_NAME} src/redis-service.cpp)

# Find redis++ and hiredis libraries
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(spdlog REQUIRED)

# Link redis++ and hiredis
target_link_libraries(${PROJECT_NAME} PRIVATE redis++::redis++ hiredis::hiredis pthread spdlog::spdlog)
