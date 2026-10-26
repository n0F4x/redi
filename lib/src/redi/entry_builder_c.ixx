module;

#include <type_traits>

export module redi.entry_builder_c;

import redi.BuildDirector;

namespace redi {

export template <typename T>
concept entry_builder_c = internal::entry_builder_c<T>;

export template <typename T>
concept decays_to_entry_builder_c = entry_builder_c<std::decay_t<T>>;

}   // namespace redi
