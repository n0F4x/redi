target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        memory.cppm
        construct_at_using_allocator.ixx
        LifeCycleEraseMechanism.ixx
        make_obj_using_allocator.ixx
        nothrow_uses_allocator_move_constructible_c.ixx
)
