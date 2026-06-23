module;

#include <cstddef>

export module redi.util.containers.MoveOnlyFunction;

import redi.util.concepts.function;
import redi.util.containers.Function;

namespace redi::util {

export using util::storable_in_function_c;

export template <
    function_c  Signature_T,
    std::size_t size_T      = default_function_size(),
    std::size_t alignment_T = default_function_alignment()>
using MoveOnlyFunction = Function<Signature_T, true, size_T, alignment_T>;

export using util::any_cast;

}   // namespace redi::util
