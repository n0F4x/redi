target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        memory.cppm
        construct_at_using_allocator.ixx
        LifeCycleEraseMechanism.ixx
)
