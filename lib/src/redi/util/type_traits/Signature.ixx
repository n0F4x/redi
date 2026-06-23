module;

#include <type_traits>

export module redi.util.type_traits.Signature;

import redi.util.TypeList;

namespace redi::util {

export template <typename>
struct Signature;

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...)> {
    using type        = Result_T(Args_T...);
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) noexcept> {
    using type        = Result_T(Args_T...) noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) &> {
    using type        = Result_T(Args_T...) &;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) & noexcept> {
    using type        = Result_T(Args_T...) & noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) &&> {
    using type        = Result_T(Args_T...) &&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) && noexcept> {
    using type        = Result_T(Args_T...) && noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const> {
    using type        = Result_T(Args_T...) const;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const noexcept> {
    using type        = Result_T(Args_T...) const noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const&> {
    using type        = Result_T(Args_T...) const&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const & noexcept> {
    using type        = Result_T(Args_T...) const& noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const&&> {
    using type        = Result_T(Args_T...) const&&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename... Args_T>
struct Signature<Result_T(Args_T...) const && noexcept> {
    using type        = Result_T(Args_T...) const&& noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...)> {
    using type        = Result_T (Class_T::*)(Args_T...);
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) &> {
    using type        = Result_T (Class_T::*)(Args_T...) &;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T&;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) & noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) & noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T&;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) &&> {
    using type        = Result_T (Class_T::*)(Args_T...) &&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T&&;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) && noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) && noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = Class_T&&;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, false> has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const> {
    using type        = Result_T (Class_T::*)(Args_T...) const;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) const noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const&> {
    using type        = Result_T (Class_T::*)(Args_T...) const&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T&;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const & noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) const& noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T&;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, true>  has_lvalue_ref{};
    constexpr static std::integral_constant<bool, false> has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const&&> {
    using type        = Result_T (Class_T::*)(Args_T...) const&&;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T&&;
    constexpr static std::integral_constant<bool, false> is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

template <typename Result_T, typename Class_T, typename... Args_T>
struct Signature<Result_T (Class_T::*)(Args_T...) const && noexcept> {
    using type        = Result_T (Class_T::*)(Args_T...) const&& noexcept;
    using arguments_t = TypeList<Args_T...>;
    using result_t    = Result_T;
    using class_t     = const Class_T&&;
    constexpr static std::integral_constant<bool, true>  is_noexcept{};
    constexpr static std::integral_constant<bool, true>  has_const{};
    constexpr static std::integral_constant<bool, false> has_lvalue_ref{};
    constexpr static std::integral_constant<bool, true>  has_rvalue_ref{};
};

}   // namespace redi::util
