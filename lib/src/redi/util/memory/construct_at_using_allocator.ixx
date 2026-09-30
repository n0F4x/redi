module;

#include <memory>
#include <type_traits>
#include <utility>

export module redi.util.memory.construct_at_using_allocator;

import redi.util.concepts.specialization_of;

namespace redi::util {

export template <typename T, typename Allocator_T, typename... Args_T>
// ReSharper disable once CppNotAllPathsReturnValue
constexpr auto construct_at_using_allocator(
    T* const           address,
    const Allocator_T& allocator,
    Args_T&&... args
) -> T*
{
    if constexpr (!std::uses_allocator_v<T, Allocator_T>)
    {
        return std::construct_at(address, std::forward<Args_T>(args)...);
    }
    else if constexpr (
        std::is_constructible_v<T, std::allocator_arg_t, const Allocator_T&, Args_T...>
    )
    {
        return std::construct_at(
            address,
            std::allocator_arg,
            allocator,
            std::forward<Args_T>(args)...
        );
    }
    else if constexpr (std::is_constructible_v<T, Args_T..., const Allocator_T&>)
    {
        return std::construct_at(address, std::forward<Args_T>(args)..., allocator);
    }
    else
    {
        static_assert(false, "invalid `uses_allocator` specialization");
    }
}

/*
 * `std::pair` is weird.
 * See https://en.cppreference.com/cpp/memory/uses_allocator_construction_args
 */
export template <typename T, typename Allocator_T, typename... Args_T>
    requires specialization_of_c<T, std::pair>
constexpr auto construct_at_using_allocator(
    T* const           address,
    const Allocator_T& allocator,
    Args_T&&... args
) -> T*
{
    return std::uninitialized_construct_using_allocator(
        address,
        allocator,
        std::forward<Args_T>(args)...
    );
}

}   // namespace redi::util
