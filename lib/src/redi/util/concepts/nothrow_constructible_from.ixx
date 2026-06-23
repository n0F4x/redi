module;

#include <concepts>

export module redi.util.concepts.nothrow_constructible_from;

namespace redi::util {

export template <typename T, typename... Args_T>
concept nothrow_constructible_from_c
    = std::destructible<T> && std::is_nothrow_constructible_v<T, Args_T...>;

}   // namespace redi::util
