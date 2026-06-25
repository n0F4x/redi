#include <optional>

#include <catch2/catch_test_macros.hpp>

import redi.util.containers.OptionalRef;

namespace redi::util {

TEST_CASE("redi::util::OptionalRef")
{
    SECTION("transform")
    {
        {
            constexpr OptionalRef<int> optional{};
            [[maybe_unused]]
            constexpr auto result{ optional.transform([](int&) -> float { return {}; }) };

            STATIC_REQUIRE(std::is_same_v<decltype(result), const std::optional<float>>);
        }

        SECTION("with value")
        {
            constexpr static int   value{ 2 };
            constexpr OptionalRef  optional{ value };
            constexpr static float other_value{ 3.2f };
            constexpr auto         result{
                optional.transform([](const int&) -> float { return other_value; })
            };

            STATIC_REQUIRE(result.value() == other_value);
        }

        SECTION("without value")
        {
            constexpr OptionalRef<int> optional{};
            constexpr static float     other_value{ 3.2f };
            constexpr auto             result{
                optional.transform([](int&) -> float { return other_value; })
            };

            STATIC_REQUIRE(!result.has_value());
        }
    }

    SECTION("or_else")
    {
        {
            constexpr OptionalRef<int> optional{};
            [[maybe_unused]]
            auto result{ optional.or_else([] -> OptionalRef<int> { return {}; }) };

            STATIC_REQUIRE(
                std::is_same_v<decltype(result), std::remove_const_t<decltype(optional)>>
            );
        }

        SECTION("with value")
        {
            constexpr static int  value{ 2 };
            constexpr OptionalRef optional{ value };
            constexpr auto        result{
                optional.or_else([] -> OptionalRef<const int> { return {}; })
            };

            STATIC_REQUIRE(*result == value);
        }

        SECTION("without value")
        {
            constexpr OptionalRef<int> optional{};
            constexpr auto             result{
                optional.or_else([] -> OptionalRef<int> { return std::nullopt; }),
            };

            STATIC_REQUIRE(result.has_value() == false);
        }
    }
}

}   // namespace redi::util
