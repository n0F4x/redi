module;

#include <memory>
#include <type_traits>
#include <utility>

export module redi.util.memory.nothrow_uses_allocator_move_constructible_c;

import redi.util.concepts.nothrow_move_constructible;
import redi.util.concepts.nothrow_constructible_from;

namespace redi::util {

namespace internal {

template <typename T, typename Allocator_T>
constexpr bool nothrow_uses_allocator_move_constructible_v{
    (!std::uses_allocator_v<T, Allocator_T> && nothrow_move_constructible_c<T>)
    || nothrow_constructible_from_c<T, std::allocator_arg_t, const Allocator_T&, T&&>
    || (!std::is_constructible_v<T, std::allocator_arg_t, const Allocator_T&, T&&>
        && nothrow_constructible_from_c<T, T&&, const Allocator_T&>)
};

/*
 * `std::pair` is weird.
 * See https://en.cppreference.com/cpp/memory/uses_allocator_construction_args
 */
template <typename First_T, typename Second_T, typename Allocator_T>
constexpr bool
    nothrow_uses_allocator_move_constructible_v<std::pair<First_T, Second_T>, Allocator_T>{
        nothrow_uses_allocator_move_constructible_v<First_T, Allocator_T>
        && nothrow_uses_allocator_move_constructible_v<Second_T, Allocator_T>
    };

}   // namespace internal

export template <typename T, typename Allocator_T>
concept nothrow_uses_allocator_move_constructible_c
    = internal::nothrow_uses_allocator_move_constructible_v<T, Allocator_T>;

}   // namespace redi::util
