#include <catch2/catch_test_macros.hpp>

import redi.util.concepts.unambiguously_invocable;

namespace redi::util {

namespace {

struct Implicit {
    // ReSharper disable once CppMemberFunctionMayBeConst
    auto operator()() -> void {}
};

struct Static {
    static auto operator()() noexcept -> void {}
};

struct Explicit {
    auto operator()(this Explicit) -> void {}
};

struct DerivedFromExplicit : Explicit {};

[[maybe_unused]]
auto free_function() -> void
{
}

constexpr auto lambda = [] -> void {};

constexpr auto static_lambda = [] static -> void {};

}   // namespace

TEST_CASE("redi::util::unambiguously_invocable_c")
{
    STATIC_REQUIRE(unambiguously_invocable_c<decltype(free_function)>);
    STATIC_REQUIRE(unambiguously_invocable_c<decltype(&free_function)>);
    STATIC_REQUIRE(unambiguously_invocable_c<Implicit>);
    STATIC_REQUIRE(unambiguously_invocable_c<Explicit>);
    STATIC_REQUIRE(unambiguously_invocable_c<DerivedFromExplicit>);
    STATIC_REQUIRE(unambiguously_invocable_c<Static>);
    STATIC_REQUIRE(unambiguously_invocable_c<decltype(lambda)>);
    STATIC_REQUIRE(unambiguously_invocable_c<decltype(static_lambda)>);
    STATIC_REQUIRE(not unambiguously_invocable_c<int>);
}

}   // namespace redi::util
