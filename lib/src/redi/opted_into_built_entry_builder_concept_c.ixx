module;

#include <concepts>

export module redi.opted_into_built_entry_builder_concept_c;

import redi.BuiltEntryBuilderTag;
import redi.opted_into_entry_builder_concept_c;

namespace redi {

export template <typename T>
concept opted_into_built_entry_builder_concept_c
    = opted_into_entry_builder_concept_c<T>
   && std::derived_from<typename T::BuilderConcept, BuiltEntryBuilderTag>;

}   // namespace redi
