module;

#include <concepts>

export module redi.opted_into_built_entry_builder_concept_c;

import redi.BuildableEntryBuilderBase;

namespace redi {

export template <typename T>
concept opted_into_built_entry_builder_concept_c
    = std::derived_from<T, internal::BuildableEntryBuilderBase>;

}   // namespace redi
