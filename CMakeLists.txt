cmake_minimum_required(VERSION 3.10)
project(EchoesOfThePast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(include)

add_executable(EchoesOfThePast src/main.cpp src/Room.cpp src/Furniture.cpp src/Drawable.cpp)

target_link_libraries(EchoesOfThePast raylib nlohmann_json::nlohmann_json)

if(UNIX)
    target_link_libraries(EchoesOfThePast pthread m dl)
endif()
