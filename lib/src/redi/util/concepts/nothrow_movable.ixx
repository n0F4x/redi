module;

#include <concepts>

export module redi.util.concepts.nothrow_movable;

import redi.util.concepts.nothrow_assignable_from;
import redi.util.concepts.nothrow_move_constructible;

namespace redi::util {

export template <typename T>
concept nothrow_movable_c = std::is_object_v<T>
                         && nothrow_move_constructible_c<T>
                         && nothrow_assignable_from_c<T&, T>
                         && std::swappable<T>;

}   // namespace redi::util
