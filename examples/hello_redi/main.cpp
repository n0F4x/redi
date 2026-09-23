#include <functional>

import redi;

struct A {
    int value{ 42 };
};

struct B {
    explicit B(A& a) : ref{ a.value } {}

    std::reference_wrapper<int> ref;
};

template <>
struct redi::EntryTraits<B> {
    static auto describe_build(BuildDirector<B>& build_director) -> void
    {
        build_director.use_dependencies<A&>();
    }
};

auto main() -> int
{
    redi::RegistryBuilder registry_builder;

    registry_builder.register_entry<B>();

    redi::Registry registry = std::move(registry_builder).build();

    return registry.at<B>().ref.get();
}
