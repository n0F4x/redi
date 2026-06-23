target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        Function.impl.hpp
)

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        containers.cppm
        Any.ixx
        CopyableAny.ixx
        CopyableFunction.ixx
        Function.ixx
        GenericStack.ixx
        MoveOnlyAny.ixx
        MoveOnlyFunction.ixx
        OptionalRef.ixx
        SmallBuffer.ixx
)
