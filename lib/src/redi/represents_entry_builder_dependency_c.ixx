module;

#include <concepts>
#include <type_traits>

export module redi.represents_entry_builder_dependency_c;

import redi.EntryBuilderBase;
import redi.EntryTraits;
import redi.util.containers.OptionalRef;

namespace redi {

// clang-format off
export template <typename T>
concept represents_entry_builder_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (std::derived_from<std::remove_cvref_t<T>, EntryBuilderBase>
           || requires { requires EntryTraits<std::remove_cvref_t<T>>::is_configuration_entry; }))
   || (util::optional_ref_c<T>
       && (std::derived_from<std::remove_const_t<typename T::value_type>, EntryBuilderBase>
           || requires { requires EntryTraits<T>::is_configuration_entry; }));
// clang-format on

}   // namespace redi
