cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME microservice)
project(${PROJECT_NAME})

# Add the executable
add_executable(microservice src/microservice.cpp)

# Find Boost libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS system thread)
find_package(spdlog REQUIRED)

# Link Boost libraries
target_link_libraries(microservice PRIVATE Boost::system Boost::thread spdlog::spdlog)
