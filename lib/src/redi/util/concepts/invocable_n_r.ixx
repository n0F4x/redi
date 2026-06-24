module;

#include <type_traits>

export module redi.util.concepts.invocable_n_r;

namespace redi::util {

export template <typename T, bool is_noexcept_T, typename Result_T, typename... Args_T>
concept invocable_n_r_c = std::conditional_t<
    is_noexcept_T,
    std::is_nothrow_invocable_r<Result_T, T, Args_T...>,
    std::is_invocable_r<Result_T, T, Args_T...>>::value;

}   // namespace redi::util
