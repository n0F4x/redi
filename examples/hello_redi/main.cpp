#include <functional>

import redi;

struct A : redi::EntryBase {
    int value{ 42 };
};

struct B;

auto describe_build(redi::BuildDirector<B>&) -> void;

struct B : redi::BuildableEntry<B, describe_build> {
    explicit B(A& a) : ref{ a.value } {}

    std::reference_wrapper<int> ref;
};

auto describe_build(redi::BuildDirector<B>& build_director) -> void
{
    build_director.use_dependencies<A&>();
}

auto main() -> int
{
    redi::RegistryBuilder registry_builder;

    registry_builder.register_entry<B>();

    redi::Registry registry = std::move(registry_builder).build();

    return registry.at<B>().ref.get();
}
