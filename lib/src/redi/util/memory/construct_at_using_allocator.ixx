module;

#include <memory>
#include <type_traits>

export module redi.util.memory.construct_at_using_allocator;

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

}   // namespace redi::util
