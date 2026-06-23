module;

#include <type_traits>

export module redi.util.concepts.unambiguous_static_functor;

import redi.util.concepts.unambiguous_explicit_functor;

namespace redi::util {

export template <typename T>
concept unambiguous_static_functor_c
    = std::is_class_v<std::remove_cvref_t<T>>   //
   && requires { &std::remove_cvref_t<T>::operator(); }
   && !std::is_member_function_pointer_v<decltype(&std::remove_cvref_t<T>::operator())>
   && !unambiguous_explicit_functor_c<T>;

}   // namespace redi::util
