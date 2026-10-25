export module redi.opted_into_configuration_entry_concept_c;

import redi.EntryTraits;

namespace redi {

export template <typename T>
concept opted_into_configuration_entry_concept_c
    = requires { requires EntryTraits<T>::is_configuration_entry; };

}   // namespace redi
