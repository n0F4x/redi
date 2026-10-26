module;

#include <concepts>

export module redi.opted_into_entry_builder_concept_c;

import redi.EntryBuilderTag;

namespace redi {

export template <typename T>
concept opted_into_entry_builder_concept_c
    = requires { typename T::BuilderConcept; }   //
   && std::derived_from<typename T::BuilderConcept, EntryBuilderTag>;

}   // namespace redi
