module;

#include <type_traits>

export module redi.util.concepts.function;

namespace redi::util {

export template <typename F>
concept function_c = std::is_function_v<F>;

}   // namespace redi::util
