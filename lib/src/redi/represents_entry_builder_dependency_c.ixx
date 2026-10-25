module;

#include <type_traits>

export module redi.represents_entry_builder_dependency_c;

import redi.EntryTraits;
import redi.opted_into_configuration_entry_concept_c;
import redi.opted_into_entry_builder_concept_c;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_builder_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (opted_into_entry_builder_concept_c<std::remove_cvref_t<T>>
           || opted_into_configuration_entry_concept_c<std::remove_cvref_t<T>>))
   || (util::optional_ref_c<T>
       && (opted_into_entry_builder_concept_c<std::remove_const_t<typename T::value_type>>
           || opted_into_configuration_entry_concept_c<
               std::remove_cvref_t<typename T::value_type>>));

}   // namespace redi
