module;

#include <concepts>

export module redi.configuration_entry_c;

import redi.entry_c;
import redi.EntryTraits;

namespace redi {

export template <typename T>
concept configuration_entry_c
    = entry_c<T>   //
   && requires { requires EntryTraits<T>::is_configuration_entry; }
   && std::default_initializable<T>;

export template <typename T>
concept decays_to_configuration_entry_c = configuration_entry_c<std::decay_t<T>>;

}   // namespace redi
