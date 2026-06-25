module;

#include <concepts>
#include <type_traits>

export module redi.represents_entry_builder_dependency_c;

import redi.configuration_entry_c;
import redi.EntryBuilderBase;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_builder_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (std::derived_from<std::remove_cvref_t<T>, EntryBuilderBase>
           || configuration_entry_c<std::remove_cvref_t<T>>))
   || (util::optional_ref_c<T>
       && (std::derived_from<std::remove_const_t<typename T::value_type>, EntryBuilderBase>
           || configuration_entry_c<std::remove_const_t<typename T::value_type>>));

}   // namespace redi
