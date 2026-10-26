#include <catch2/catch_test_macros.hpp>

import redi.util.concepts.unambiguous_implicit_functor;

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

TEST_CASE("redi::util::unambiguous_implicit_functor_c")
{
    STATIC_REQUIRE(unambiguous_implicit_functor_c<Implicit>);
    STATIC_REQUIRE(unambiguous_implicit_functor_c<decltype(lambda)>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<Explicit>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<DerivedFromExplicit>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<Static>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<decltype(static_lambda)>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<decltype(free_function)>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<decltype(&free_function)>);
    STATIC_REQUIRE(not unambiguous_implicit_functor_c<int>);
}

}   // namespace redi::util
