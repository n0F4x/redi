module;

#include <concepts>

export module redi.opted_into_entry_builder_concept_c;

import redi.EntryBuilderBase;

namespace redi {

export template <typename T>
concept opted_into_entry_builder_concept_c = std::derived_from<T, EntryBuilderBase>;

}   // namespace redi
