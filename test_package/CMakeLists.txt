cmake_minimum_required(VERSION 4.3)
project(test_package)

find_package(redi CONFIG REQUIRED)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE redi::redi)
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)
