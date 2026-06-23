module;

#include <type_traits>

export module redi.util.concepts.function_pointer;

namespace redi::util {

export template <typename T>
concept function_pointer_c
    = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>;

}   // namespace redi::util
