module;

#include <cstdint>
#include <string>
#include <string_view>

export module redi.DependencyChainNode;

namespace redi {

export struct DependencyChainNode {
    const DependencyChainNode* previous{};
    uint64_t                   hash;
    std::string_view           name;

    [[nodiscard]]
    auto contains(uint64_t hash) const noexcept -> bool;

    [[nodiscard]]
    auto format(const std::pmr::string::allocator_type& allocator) const
        -> std::pmr::string;

private:
    auto format(std::pmr::string& out, std::size_t capacity) const -> void;
};

}   // namespace redi
