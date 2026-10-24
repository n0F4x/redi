module;

#include <type_traits>

export module redi.represents_entry_builder_dependency_c;

import redi.EntryTraits;
import redi.opted_into_entry_builder_concept_c;
import redi.util.containers.OptionalRef;

namespace redi {

// clang-format off
export template <typename T>
concept represents_entry_builder_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (opted_into_entry_builder_concept_c<std::remove_cvref_t<T>>
           || requires { requires EntryTraits<std::remove_cvref_t<T>>::is_configuration_entry; }))
   || (util::optional_ref_c<T>
       && (opted_into_entry_builder_concept_c<std::remove_const_t<typename T::value_type>>
           || requires {
                  requires EntryTraits<
                      std::remove_const_t<typename T::value_type>>::is_configuration_entry;
              }));
// clang-format on

}   // namespace redi
