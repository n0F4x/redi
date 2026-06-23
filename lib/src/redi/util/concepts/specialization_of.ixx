module;

#include <type_traits>

export module redi.util.concepts.specialization_of;

namespace redi::util {

namespace internal {

template <typename, template <typename...> typename>
struct IsSpecializationOf : std::false_type {};

template <template <typename...> typename TypeList_T, typename... Ts>
struct IsSpecializationOf<TypeList_T<Ts...>, TypeList_T> : std::true_type {};

}   // namespace internal

export template <typename T, template <typename...> typename TypeList_T>
concept specialization_of_c = internal::IsSpecializationOf<T, TypeList_T>::value;

}   // namespace redi::util
