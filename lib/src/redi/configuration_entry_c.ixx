module;

#include <concepts>

export module redi.configuration_entry_c;

import redi.entry_c;
import redi.opted_into_built_entry_concept_c;
import redi.opted_into_configuration_entry_concept_c;

namespace redi {

export template <typename T>
concept configuration_entry_c = entry_c<T>
                             && opted_into_configuration_entry_concept_c<T>
                             && !opted_into_built_entry_concept_c<T>
                             && std::default_initializable<T>;

export template <typename T>
concept decays_to_configuration_entry_c = configuration_entry_c<std::decay_t<T>>;

}   // namespace redi
