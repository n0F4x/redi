export module redi.util.concepts.nothrow_move_constructible;

import redi.util.concepts.nothrow_constructible_from;
import redi.util.concepts.nothrow_convertible_to;

namespace redi::util {

export template <typename T>
concept nothrow_move_constructible_c
    = nothrow_constructible_from_c<T, T> && nothrow_convertible_to_c<T, T>;

}   // namespace redi::util
