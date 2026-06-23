module;

#include <cstddef>

export module redi.util.TypeList;

namespace redi::util {

export template <typename... Ts>
struct TypeList {
    [[nodiscard]]
    consteval static auto size() noexcept -> std::size_t
    {
        return sizeof...(Ts);
    }
};

}   // namespace redi::util
