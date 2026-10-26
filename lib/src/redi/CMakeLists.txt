add_subdirectory(util)

target_sources(${PROJECT_NAME} PRIVATE
        DependencyChainNode.cxx
        EntryBuilderContainer.cxx
        EntryInjectionContainer.cxx
        RegistryBuilder.cxx
)

target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${PROJECT_SOURCE_DIR}/src
        FILES
        redi.cppm
        BuildDirector.ixx
        BuiltEntryBuilderTag.ixx
        configuration_entry_c.ixx
        CyclicDependencyDetected.ixx
        DependencyChainNode.ixx
        entry_c.ixx
        entry_builder_c.ixx
        EntryBuilderContainer.ixx
        EntryBuilderFailedError.ixx
        EntryBuilderTag.ixx
        EntryInjectionContainer.ixx
        EntryTraits.ixx
        ErasedEntry.ixx
        opted_into_built_entry_builder_concept_c.ixx
        opted_into_built_entry_concept_c.ixx
        opted_into_configuration_entry_concept_c.ixx
        opted_into_entry_concept_c.ixx
        opted_into_entry_builder_concept_c.ixx
        Registry.ixx
        RegistryBuilder.ixx
        represents_entry_builder_dependency_c.ixx
        represents_entry_dependency_c.ixx
        strip_dependency_t.ixx
)
