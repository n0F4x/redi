module;

#include <type_traits>

export module redi.util.concepts.unambiguously_invocable;

import redi.util.concepts.function;
import redi.util.concepts.function_pointer;
import redi.util.concepts.function_reference;
import redi.util.concepts.member_function_pointer;
import redi.util.concepts.unambiguous_functor;

namespace redi::util {

export template <typename T>
concept unambiguously_invocable_c = function_c<T>                                    //
                                 || function_pointer_c<std::remove_reference_t<T>>   //
                                 || function_reference_c<T>
                                 || member_function_pointer_c<T>
                                 || unambiguous_functor_c<T>;

}   // namespace redi::util
