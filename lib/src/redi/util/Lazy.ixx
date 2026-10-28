module;

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

export module redi.util.Lazy;

import redi.util.concepts.decayed;

namespace redi::util {

export template <decayed_c F>
    requires std::invocable<F&&> && (!std::is_void_v<std::invoke_result_t<F&&>>)
class Lazy {
public:
    constexpr explicit Lazy(F&& func) noexcept(std::is_nothrow_move_constructible_v<F>)
        : m_func{ std::move(func) }
    {
    }

    [[nodiscard]]
    constexpr explicit(false) operator std::invoke_result_t<F&&>() &&   //
        noexcept(std::is_nothrow_invocable_v<F&&>)
    {
        return std::invoke(std::move(m_func));
    }

private:
    F m_func;
};

}   // namespace redi::util
