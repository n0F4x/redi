module;

#include <memory>
#include <type_traits>
#include <utility>

export module redi.util.memory.make_obj_using_allocator;

namespace redi::util {

export template <typename T, typename Allocator_T, typename... Args_T>
[[nodiscard]]
// ReSharper disable once CppNotAllPathsReturnValue
constexpr auto make_obj_using_allocator(const Allocator_T& allocator, Args_T&&... args)
    -> T
{
    if constexpr (!std::uses_allocator_v<T, Allocator_T>)
    {
        return T(std::forward<Args_T>(args)...);
    }
    else if constexpr (
        std::is_constructible_v<T, std::allocator_arg_t, const Allocator_T&, Args_T...>
    )
    {
        return T(std::allocator_arg, allocator, std::forward<Args_T>(args)...);
    }
    else if constexpr (std::is_constructible_v<T, Args_T..., const Allocator_T&>)
    {
        return T(std::forward<Args_T>(args)..., allocator);
    }
    else
    {
        static_assert(false, "invalid `uses_allocator` specialization");
    }
}

}   // namespace redi::util
