#include <concepts>
#include <optional>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

import redi.util.Lazy;

namespace redi::util {

namespace {

struct Immovable {
    int value;

    constexpr explicit Immovable(const int value) noexcept : value{ value } {}

    Immovable(const Immovable&) = delete;
    Immovable(Immovable&&)      = delete;
};

}   // namespace

TEST_CASE("redi::util::Lazy")
{
    SECTION("constructs immovable types in place")
    {
        constexpr auto make_non_movable{
            [] -> Immovable { return Immovable{ 2 }; },
        };

        STATIC_REQUIRE(
            std::constructible_from<
                Immovable,
                Lazy<std::remove_const_t<decltype(make_non_movable)>>>
        );

        std::optional<Immovable> optional;
        optional.emplace(Lazy{ auto{ make_non_movable } });

        REQUIRE(optional.has_value());
        REQUIRE(optional->value == 2);
    }
}

}   // namespace redi::util
