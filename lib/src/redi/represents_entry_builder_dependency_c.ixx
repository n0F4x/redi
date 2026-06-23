module;

#include <concepts>
#include <type_traits>

export module redi.represents_entry_builder_dependency_c;

import redi.configuration_entry_c;
import redi.EntryBuilderBase;
import redi.util.concepts.specialization_of;
import redi.util.containers.OptionalRef;

namespace redi {

export template <typename T>
concept represents_entry_builder_dependency_c
    = (std::is_lvalue_reference_v<T>
       && (std::derived_from<std::remove_cvref_t<T>, EntryBuilderBase>
           || configuration_entry_c<std::remove_cvref_t<T>>))
   || (util::specialization_of_c<T, util::OptionalRef>
       && (std::derived_from<std::remove_cvref_t<typename T::ValueType>, EntryBuilderBase>
           || configuration_entry_c<std::remove_cvref_t<typename T::ValueType>>));

}   // namespace redi
