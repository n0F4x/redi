module;

#include <concepts>
#include <type_traits>

export module redi.represents_entry_dependency_c;

import redi.entry_c;
import redi.EntryBuilderBase;
import redi.util.concepts.specialization_of;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (entry_c<std::remove_cvref_t<T>>
           || (std::derived_from<std::remove_cvref_t<T>, EntryBuilderBase>
               && std::is_const_v<std::remove_reference_t<T>>)))
   || (util::specialization_of_c<T, util::OptionalRef>
       && (entry_c<std::remove_cvref_t<typename T::ValueType>>
           || (std::derived_from<std::remove_cvref_t<typename T::ValueType>, EntryBuilderBase>
               && std::is_const_v<std::remove_reference_t<typename T::ValueType>>)));

}   // namespace redi
