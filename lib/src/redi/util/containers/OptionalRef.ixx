module;

#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <version>

#include "redi/util/contract_macros.hpp"

export module redi.util.containers.OptionalRef;

import redi.util.contracts;

// clang-format off
#if __cpp_lib_optional >= 202506L
// clang-format on

namespace redi::util {

export template <typename T>
using OptionalRef = std::optional<T&>;

namespace internal {

template <typename T>
constexpr bool optional_ref_v{ false };

template <typename T>
constexpr bool optional_ref_v<OptionalRef<T>>{ true };

}   // namespace internal

export template <typename T>
concept optional_ref_c = internal::optional_ref_v<T>;

}   // namespace redi::util
#else
namespace redi::util {

export template <typename T>
    requires(!std::is_reference_v<T>)
class OptionalRef;

namespace internal {

template <typename F, typename T>
concept and_then_func_c
    = !std::is_reference_v<T>
   && std::constructible_from<std::invoke_result_t<F&&, T&>, std::nullopt_t>;

template <typename F, typename T>
    requires(std::is_reference_v<T>)
using transform_result_t = std::conditional_t<
    std::is_reference_v<std::invoke_result_t<F, T>>,
    OptionalRef<std::remove_reference_t<std::invoke_result_t<F, T>>>,
    std::optional<std::invoke_result_t<F, T>>>;

template <typename F, typename T>
concept transform_func_c = std::invocable<F&&, T&>;

template <typename F, typename T>
concept or_else_func_c = std::constructible_from<std::invoke_result_t<F&&>, T&>;

}   // namespace internal

export template <typename T>
    requires(!std::is_reference_v<T>)
class OptionalRef {
public:
    using value_type = T;

    OptionalRef() = default;
    constexpr explicit(false) OptionalRef(std::nullopt_t) noexcept;
    template <std::same_as<std::remove_const_t<T>> U>
        requires(std::is_const_v<T>)
    constexpr explicit(false) OptionalRef(OptionalRef<U> other) noexcept;
    constexpr explicit(false) OptionalRef(T& ref) noexcept;
    constexpr explicit(false) OptionalRef(
        const std::optional<std::reference_wrapper<T>>& optional_ref_wrapper
    ) noexcept;

    constexpr explicit operator std::optional<std::remove_const_t<T>>() noexcept(
        noexcept(T{ std::declval<T&>() })
    );

    [[nodiscard]]
    constexpr auto operator->() const -> T*;
    [[nodiscard]]
    constexpr auto operator*() const -> T&;

    [[nodiscard]]
    constexpr auto has_value() const noexcept -> bool;

    template <typename U>
        requires std::convertible_to<U&&, T&>
    [[nodiscard]]
    constexpr auto value_or(U&& other) const noexcept -> T&;

    template <internal::and_then_func_c<T> F>
    constexpr auto and_then(F&& func) const -> std::invoke_result_t<F&&, T&>;
    template <internal::transform_func_c<T> F>
    constexpr auto transform(F&& func) const -> internal::transform_result_t<F&&, T&>;
    template <internal::or_else_func_c<T> F>
    constexpr auto or_else(F&& func) const -> std::invoke_result_t<F&&>;

private:
    T* m_handle{};
};

namespace internal {

template <typename T>
constexpr bool optional_ref_v{ false };

template <typename T>
constexpr bool optional_ref_v<OptionalRef<T>>{ true };

}   // namespace internal

export template <typename T>
concept optional_ref_c = internal::optional_ref_v<T>;

}   // namespace redi::util

namespace redi::util {

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr OptionalRef<T>::OptionalRef(std::nullopt_t) noexcept : OptionalRef{}
{
}

template <typename T>
    requires(!std::is_reference_v<T>)
template <std::same_as<std::remove_const_t<T>> U>
    requires(std::is_const_v<T>)
constexpr OptionalRef<T>::OptionalRef(const OptionalRef<U> other) noexcept
    : m_handle{
          other
              .transform([](T& value) -> T* { return std::addressof(value); })
              .value_or(nullptr)   //
      }
{
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr OptionalRef<T>::OptionalRef(T& ref) noexcept : m_handle{ std::addressof(ref) }
{
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr OptionalRef<T>::OptionalRef(
    const std::optional<std::reference_wrapper<T>>& optional_ref_wrapper
) noexcept
    : m_handle{
          optional_ref_wrapper
              .transform([](T& ref) -> T* { return std::addressof(ref); })
              .value_or(nullptr)   //
      }
{
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr OptionalRef<T>::operator std::optional<std::remove_const_t<T>>() noexcept(
    noexcept(T{ std::declval<T&>() })
)
{
    if (has_value())
    {
        return std::optional<T>{ *m_handle };
    }
    return std::optional<T>{};
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr auto OptionalRef<T>::operator->() const -> T*
{
    PRECOND(m_handle != nullptr);
    return m_handle;
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr auto OptionalRef<T>::operator*() const -> T&
{
    PRECOND(m_handle != nullptr);
    // ReSharper disable once CppDFANullDereference
    return *m_handle;
}

template <typename T>
    requires(!std::is_reference_v<T>)
constexpr auto OptionalRef<T>::has_value() const noexcept -> bool
{
    return m_handle != nullptr;
}

template <typename T>
    requires(!std::is_reference_v<T>)
template <typename U>
    requires std::convertible_to<U&&, T&>
constexpr auto OptionalRef<T>::value_or(U&& other) const noexcept -> T&
{
    if (has_value())
    {
        return *m_handle;
    }
    return std::forward<U>(other);
}

template <typename T>
    requires(!std::is_reference_v<T>)
template <internal::and_then_func_c<T> F>
constexpr auto OptionalRef<T>::and_then(F&& func) const -> std::invoke_result_t<F&&, T&>
{
    if (has_value())
    {
        return std::invoke(std::forward<F>(func), *m_handle);
    }
    return std::nullopt;
}

template <typename T>
    requires(!std::is_reference_v<T>)
template <internal::transform_func_c<T> F>
constexpr auto OptionalRef<T>::transform(F&& func) const
    -> internal::transform_result_t<F&&, T&>
{
    if (has_value())
    {
        return internal::transform_result_t<F&&, T&>{
            std::invoke(std::forward<F>(func), *m_handle)
        };
    }
    return std::nullopt;
}

template <typename T>
    requires(!std::is_reference_v<T>)
template <internal::or_else_func_c<T> F>
constexpr auto OptionalRef<T>::or_else(F&& func) const -> std::invoke_result_t<F&&>
{
    if (!has_value())
    {
        return std::invoke(std::forward<F>(func));
    }
    return std::invoke_result_t<F&&>{ **this };
}

}   // namespace redi::util
#endif
