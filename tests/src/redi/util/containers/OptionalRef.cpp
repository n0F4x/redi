#include <expected>
#include <optional>

#include <catch2/catch_test_macros.hpp>

import redi.util.containers.OptionalRef;

namespace redi::util {

TEST_CASE("util::OptionalRef")
{
    SECTION("transform")
    {
        {
            constexpr OptionalRef<int> optional_ref{};
            [[maybe_unused]]
            constexpr auto optional{
                optional_ref.transform([](int&) -> float { return {}; })
            };

            STATIC_REQUIRE(std::is_same_v<decltype(optional), const std::optional<float>>);
        }

        SECTION("with value")
        {
            constexpr static int   value{ 2 };
            constexpr OptionalRef  optional_ref{ value };
            constexpr static float other_value{ 3.2f };
            constexpr auto         optional{
                optional_ref.transform([](const int&) -> float { return other_value; })
            };

            STATIC_REQUIRE(optional.value() == other_value);
        }

        SECTION("without value")
        {
            constexpr OptionalRef<int> optional_ref{};
            constexpr static float     other_value{ 3.2f };
            constexpr auto             optional{
                optional_ref.transform([](int&) -> float { return other_value; })
            };

            STATIC_REQUIRE(!optional.has_value());
        }
    }

    SECTION("or_else")
    {
        {
            constexpr OptionalRef<int> optional{};
            [[maybe_unused]]
            auto expected{
                optional.or_else([] -> std::expected<int, float> { return {}; })
            };

            STATIC_REQUIRE(std::is_same_v<decltype(expected), std::expected<int, float>>);
        }

        SECTION("with value")
        {
            constexpr static int  value{ 2 };
            constexpr OptionalRef optional{ value };
            constexpr auto        expected{
                optional.or_else([] -> std::expected<int, float> { return {}; })
            };

            STATIC_REQUIRE(expected.value() == value);
        }

        SECTION("without value")
        {
            constexpr OptionalRef<int> optional{};
            constexpr static float     error{ 6.7f };
            constexpr auto             expected{ optional.or_else(
                [] -> std::expected<int, float> { return std::unexpected{ error }; }
            ) };

            STATIC_REQUIRE(expected.error() == error);
        }
    }
}

}   // namespace redi::util
