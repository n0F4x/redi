project(tests)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE redi::redi)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

add_subdirectory(src)

include(Catch)
catch_discover_tests(${PROJECT_NAME})
