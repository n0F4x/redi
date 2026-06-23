module;

#include <type_traits>

export module redi.util.concepts.member_function_pointer;

namespace redi::util {

export template <typename F>
concept member_function_pointer_c = std::is_member_function_pointer_v<F>;

}   // namespace redi::util
