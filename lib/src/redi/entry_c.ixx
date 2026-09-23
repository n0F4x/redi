module;

#include <concepts>
#include <type_traits>

export module redi.entry_c;

import redi.EntryBuilderBase;
import redi.util.containers.MoveOnlyAny;

namespace redi {

export template <typename T>
concept entry_c = util::storable_in_any_c<T, util::BasicMoveOnlyAny<0>>
               && !std::derived_from<T, EntryBuilderBase>;

export template <typename T>
concept decays_to_entry_c = entry_c<std::decay_t<T>>;

}   // namespace redi
