module;

#include <type_traits>

export module redi.util.type_traits.result_of;

import redi.util.concepts.function;
import redi.util.concepts.function_pointer;
import redi.util.concepts.function_reference;
import redi.util.concepts.member_function_pointer;
import redi.util.concepts.unambiguous_functor;
import redi.util.concepts.unambiguously_invocable;
import redi.util.type_traits.Signature;

namespace redi::util {

namespace internal {

template <unambiguously_invocable_c F>
struct ResultOf;

template <function_c F>
struct ResultOf<F> {
    using type = typename Signature<F>::result_t;
};

template <typename F>
    requires function_pointer_c<std::remove_reference_t<F>>
struct ResultOf<F> {
    using type =
        typename Signature<std::remove_pointer_t<std::remove_reference_t<F>>>::result_t;
};

template <function_reference_c F>
struct ResultOf<F> {
    using type = typename Signature<std::remove_reference_t<F>>::result_t;
};

template <member_function_pointer_c F>
struct ResultOf<F> {
    using type = typename Signature<F>::result_t;
};

template <unambiguous_functor_c F>
struct ResultOf<F> {
    using type = typename ResultOf<decltype(&std::remove_cvref_t<F>::operator())>::type;
};

}   // namespace internal

export template <typename F>
using result_of_t = typename internal::ResultOf<F>::type;

}   // namespace redi::util
