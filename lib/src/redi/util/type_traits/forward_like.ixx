module;

#include <utility>

export module redi.util.type_traits.forward_like;

namespace redi::util {

export template <typename T, typename Like_T>
using forward_like_t = decltype(std::forward_like<Like_T>(std::declval<T>()));

}   // namespace redi::util
