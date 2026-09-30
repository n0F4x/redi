module;

#include <exception>
#include <functional>
#include <type_traits>

export module redi.util.ScopeSuccess;

import redi.util.concepts.storable;

namespace redi::util {

export template <storable_c CleanUp_T>
    requires(std::is_nothrow_invocable_v<CleanUp_T>)
class [[nodiscard]]
ScopeSuccess {
public:
    constexpr explicit(false) ScopeSuccess(const CleanUp_T& clean_up) noexcept
        requires(std::is_nothrow_constructible_v<CleanUp_T, const CleanUp_T&>);
    constexpr explicit(false) ScopeSuccess(CleanUp_T&& clean_up) noexcept
        requires(std::is_nothrow_constructible_v<CleanUp_T, CleanUp_T&&>);
    ScopeSuccess(const ScopeSuccess&) = delete;
    ScopeSuccess(ScopeSuccess&&)      = default;
    constexpr ~ScopeSuccess();

private:
    CleanUp_T m_clean_up;
    int       m_uncaught_exceptions{
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

template <storable_c CleanUp_T>
    requires(std::is_nothrow_invocable_v<CleanUp_T>)
constexpr ScopeSuccess<CleanUp_T>::~ScopeSuccess()
{
#ifndef __cpp_constexpr_exceptions
    if consteval
    {
        std::invoke(m_clean_up);
    }
    else
    {
#endif
        if (m_uncaught_exceptions == std::uncaught_exceptions())
        {
            std::invoke(m_clean_up);
        }
#ifndef __cpp_constexpr_exceptions
    }
#endif
}

template <storable_c CleanUp_T>
    requires(std::is_nothrow_invocable_v<CleanUp_T>)
constexpr ScopeSuccess<CleanUp_T>::ScopeSuccess(const CleanUp_T& clean_up) noexcept
    requires(std::is_nothrow_constructible_v<CleanUp_T, const CleanUp_T&>)
    : m_clean_up{ clean_up }
{
}

template <storable_c CleanUp_T>
    requires(std::is_nothrow_invocable_v<CleanUp_T>)
constexpr ScopeSuccess<CleanUp_T>::ScopeSuccess(CleanUp_T&& clean_up) noexcept
    requires(std::is_nothrow_constructible_v<CleanUp_T, CleanUp_T&&>)
    : m_clean_up{ std::move(clean_up) }
{
}

}   // namespace redi::util
