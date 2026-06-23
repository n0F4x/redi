add_subdirectory(concepts)
add_subdirectory(containers)
add_subdirectory(type_traits)

target_sources(${PROJECT_NAME} PRIVATE
        contracts.cxx
)

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        contract_macros.hpp
)

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        util.cppm
        any_cast.ixx
        contracts.ixx
        for_each.ixx
        LifeCycleEraseMechanism.ixx
        reflection.ixx
        ScopeFail.ixx
        TypeList.ixx
)
