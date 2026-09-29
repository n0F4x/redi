cmake_minimum_required(VERSION 4.3)

project(redi VERSION 0.0.0)

set(project_prefix REDI_)

option(${project_prefix}DEBUG "Activate debug functionalities" OFF)


add_library(${PROJECT_NAME})
add_library(redi::redi ALIAS ${PROJECT_NAME})


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        ${project_prefix}VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        ${project_prefix}VERSION_MINOR=${PROJECT_VERSION_MINOR}
        ${project_prefix}VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if (${project_prefix}DEBUG)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
            ${project_prefix}DEBUG
    )
endif ()


add_subdirectory(src)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT rediTargets
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET CXX_MODULES
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(EXPORT rediTargets
        FILE rediTargets.cmake
        NAMESPACE redi::
        CXX_MODULES_DIRECTORY cxx-modules
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/redi
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/rediConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/redi
)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/rediConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/rediConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/rediConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/redi
)
