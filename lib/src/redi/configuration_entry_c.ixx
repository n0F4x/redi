module;

#include <concepts>

export module redi.configuration_entry_c;

import redi.BuildDirector;
import redi.entry_c;
import redi.EntryTraits;

namespace redi {

export template <typename T>
concept configuration_entry_c
    = entry_c<T>   //
   && requires { requires EntryTraits<T>::is_configuration_entry; }
   && not(requires(BuildDirector<T> build_director) {
          EntryTraits<T>::describe_build(build_director);
      })
   && std::default_initializable<T>;

export template <typename T>
concept decays_to_configuration_entry_c = configuration_entry_c<std::decay_t<T>>;

}   // namespace redi
