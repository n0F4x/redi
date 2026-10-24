module;

#include <type_traits>

export module redi.entry_c;

import redi.util.containers.MoveOnlyAny;
import redi.opted_into_entry_builder_concept_c;

namespace redi {

export template <typename T>
concept entry_c = util::storable_in_any_c<T, util::BasicMoveOnlyAny<0>>
               && !opted_into_entry_builder_concept_c<T>;

export template <typename T>
concept decays_to_entry_c = entry_c<std::decay_t<T>>;

}   // namespace redi
