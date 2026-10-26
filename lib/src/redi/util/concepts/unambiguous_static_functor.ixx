module;

#include <type_traits>
#include <utility>

export module redi.util.concepts.unambiguous_static_functor;

import redi.util.TypeList;
import redi.util.type_traits.Signature;

namespace redi::util {

namespace internal {

template <typename T, typename... Args_T>
consteval auto invocable_without_object(TypeList<Args_T...>) -> bool
{
    return requires { T::operator()(std::declval<Args_T>()...); };
}

}   // namespace internal

export template <typename T>
concept unambiguous_static_functor_c
    = std::is_class_v<std::remove_cvref_t<T>>
   && requires { &std::remove_cvref_t<T>::operator(); }
   && !std::is_member_function_pointer_v<decltype(&std::remove_cvref_t<T>::operator())>
   && internal::invocable_without_object<std::remove_cvref_t<T>>(
          typename Signature<std::remove_pointer_t<
              decltype(&std::remove_cvref_t<T>::operator())>>::arguments_t{}
   );

}   // namespace redi::util
