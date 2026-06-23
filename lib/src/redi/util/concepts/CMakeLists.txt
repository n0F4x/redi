target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        concepts.cppm
        decayed.ixx
        function.ixx
        function_pointer.ixx
        function_reference.ixx
        member_function_pointer.ixx
        nothrow_assignable_from.ixx
        nothrow_constructible_from.ixx
        nothrow_convertible_to.ixx
        nothrow_movable.ixx
        nothrow_move_constructible.ixx
        specialization_of.ixx
        storable.ixx
        strips_to.ixx
        unambiguous_explicit_functor.ixx
        unambiguous_functor.ixx
        unambiguous_implicit_functor.ixx
        unambiguous_static_functor.ixx
        unambiguously_invocable.ixx
)
