module;

#include <concepts>
#include <type_traits>

export module redi.util.concepts.decayed;

namespace redi::util {

export template <typename T>
concept decayed_c = std::same_as<T, std::decay_t<T>>;

}   // namespace redi::util
