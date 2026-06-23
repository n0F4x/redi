module;

#include <type_traits>

export module redi.util.concepts.unambiguous_explicit_functor;

import redi.util.type_traits.Signature;
import redi.util.type_traits.type_list_front;

namespace redi::util {

export template <typename T>
concept unambiguous_explicit_functor_c
    = std::is_class_v<std::remove_cvref_t<T>>
   && requires { &std::remove_cvref_t<T>::operator(); }
   && !std::is_member_function_pointer_v<decltype(&std::remove_cvref_t<T>::operator())>
   && std::is_same_v<
          std::remove_cvref_t<type_list_front_t<typename Signature<std::remove_pointer_t<
              decltype(&std::remove_cvref_t<T>::operator())>>::arguments_t>>,
          std::remove_cvref_t<T>>;

}   // namespace redi::util
