#include <type_traits>

import redi.util.type_traits.type_list_drop_front;

namespace {

template <typename...>
struct TypeList {};

}   // namespace

static_assert(
    std::is_same_v<redi::util::type_list_drop_front_t<TypeList<float>>, TypeList<>>
);
static_assert(
    std::is_same_v<redi::util::type_list_drop_front_t<TypeList<int, float>>, TypeList<float>>
);
