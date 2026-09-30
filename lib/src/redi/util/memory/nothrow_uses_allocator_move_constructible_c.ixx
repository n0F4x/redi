module;

#include <memory>

export module redi.util.memory.nothrow_uses_allocator_move_constructible_c;

import redi.util.concepts.nothrow_move_constructible;
import redi.util.concepts.nothrow_constructible_from;

namespace redi::util {

export template <typename T, typename Allocator_T>
concept nothrow_uses_allocator_move_constructible_c
    = (!std::uses_allocator_v<T, Allocator_T> && nothrow_move_constructible_c<T>)
   || nothrow_constructible_from_c<T, std::allocator_arg_t, const Allocator_T&, T&&>
   || (!std::is_constructible_v<T, std::allocator_arg_t, const Allocator_T&, T&&>
       && nothrow_constructible_from_c<T, T&&, const Allocator_T&>);

}   // namespace redi::util
