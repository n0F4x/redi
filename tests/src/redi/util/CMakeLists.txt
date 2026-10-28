add_subdirectory(concepts)
add_subdirectory(containers)
add_subdirectory(type_traits)

target_sources(${PROJECT_NAME} PRIVATE
        Lazy.cpp
)
