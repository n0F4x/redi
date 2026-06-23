module;

#include <array>
#include <exception>
#include <functional>
#include <type_traits>

export module redi.util.ScopeFail;

import redi.util.concepts.storable;

namespace redi::util {

export template <storable_c Rollback_T>
    requires(std::is_nothrow_invocable_v<Rollback_T>)
class [[nodiscard]]
ScopeFail {
public:
    constexpr explicit(false) ScopeFail(const Rollback_T& rollback) noexcept
        requires(std::is_nothrow_constructible_v<Rollback_T, const Rollback_T&>);
    constexpr explicit(false) ScopeFail(Rollback_T&& rollback) noexcept
        requires(std::is_nothrow_constructible_v<Rollback_T, Rollback_T &&>);
    ScopeFail(const ScopeFail&) = delete;
    ScopeFail(ScopeFail&&)      = default;
    constexpr ~ScopeFail();

private:
    Rollback_T m_rollback;
    int        m_uncaught_exceptions{
        []
        {
            return
#ifndef __cpp_constexpr_exceptions
                std::is_constant_evaluated() ? 0 :
#endif
                                             std::uncaught_exceptions();
        }(),
    };
};

}   // namespace redi::util

namespace redi::util {

template <storable_c Rollback_T>
    requires(std::is_nothrow_invocable_v<Rollback_T>)
constexpr ScopeFail<Rollback_T>::~ScopeFail<Rollback_T>()
{
#ifndef __cpp_constexpr_exceptions
    if !consteval
    {
#endif
        if (m_uncaught_exceptions > std::uncaught_exceptions())
        {
            std::invoke(m_rollback);
        }
#ifndef __cpp_constexpr_exceptions
    }
#endif
}

template <storable_c Rollback_T>
    requires(std::is_nothrow_invocable_v<Rollback_T>)
constexpr ScopeFail<Rollback_T>::ScopeFail(const Rollback_T& rollback) noexcept
    requires(std::is_nothrow_constructible_v<Rollback_T, const Rollback_T&>)
    : m_rollback{ rollback }
{
}

template <storable_c Rollback_T>
    requires(std::is_nothrow_invocable_v<Rollback_T>)
constexpr ScopeFail<Rollback_T>::ScopeFail(Rollback_T&& rollback) noexcept
    requires(std::is_nothrow_constructible_v<Rollback_T, Rollback_T &&>)
    : m_rollback{ std::move(rollback) }
{
}

}   // namespace redi::util
