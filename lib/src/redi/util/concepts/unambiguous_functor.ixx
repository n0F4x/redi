module;

export module redi.util.concepts.unambiguous_functor;

import redi.util.concepts.unambiguous_explicit_functor;
import redi.util.concepts.unambiguous_implicit_functor;
import redi.util.concepts.unambiguous_static_functor;

namespace redi::util {

export template <typename T>
concept unambiguous_functor_c = unambiguous_explicit_functor_c<T>
                             || unambiguous_implicit_functor_c<T>
                             || unambiguous_static_functor_c<T>;

}   // namespace redi::util
