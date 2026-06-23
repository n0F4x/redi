module;

#include <concepts>
#include <type_traits>

export module redi.util.concepts.storable;

namespace redi::util {

export template <typename T>
concept storable_c = !std::is_abstract_v<T> && std::destructible<T>;

}   // namespace redi::util
