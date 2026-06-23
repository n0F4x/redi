project(hello_redi)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE redi::redi)

target_sources(${PROJECT_NAME} PRIVATE
        main.cpp
)
