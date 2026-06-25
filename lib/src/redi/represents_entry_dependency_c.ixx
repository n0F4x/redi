module;

#include <concepts>
#include <type_traits>

export module redi.represents_entry_dependency_c;

import redi.entry_c;
import redi.EntryBuilderBase;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (entry_c<std::remove_cvref_t<T>>
           || (std::derived_from<std::remove_cvref_t<T>, EntryBuilderBase>
               && std::is_const_v<std::remove_reference_t<T>>)))
   || (util::optional_ref_c<T>
       && (entry_c<std::remove_const_t<typename T::value_type>>
           || (std::derived_from<std::remove_const_t<typename T::value_type>, EntryBuilderBase>
               && std::is_const_v<typename T::value_type>)));

}   // namespace redi
