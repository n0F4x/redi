module;

#include <type_traits>

export module redi.represents_entry_dependency_c;

import redi.entry_c;
import redi.EntryBuilderBase;
import redi.opted_into_entry_builder_concept_c;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (entry_c<std::remove_cvref_t<T>>
           || (opted_into_entry_builder_concept_c<std::remove_cvref_t<T>>
               && std::is_const_v<std::remove_reference_t<T>>)))
   || (util::optional_ref_c<T>
       && (entry_c<std::remove_const_t<typename T::value_type>>
           || (opted_into_entry_builder_concept_c<
                   std::remove_const_t<typename T::value_type>>
               && std::is_const_v<typename T::value_type>)));

}   // namespace redi
