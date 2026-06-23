module;

#include <concepts>
#include <memory_resource>

export module redi.RegistryBuilder;

import redi.BuildableEntryBase;
import redi.BuildDirector;
import redi.entry_c;
import redi.EntryBuilderContainer;
import redi.EntryInjectionContainer;
import redi.Registry;

namespace redi {

export class RegistryBuilder {
public:
    using allocator_type = std::pmr::polymorphic_allocator<>;


    RegistryBuilder(const RegistryBuilder&)     = delete;
    RegistryBuilder(RegistryBuilder&&) noexcept = default;
    RegistryBuilder(RegistryBuilder&&, const allocator_type&);

    explicit RegistryBuilder() = default;
    explicit RegistryBuilder(const allocator_type&);

    template <decays_to_entry_c... Entries_T>
    explicit RegistryBuilder(
        std::allocator_arg_t,
        const allocator_type&,
        Entries_T&&... entries
    );

    auto operator=(const RegistryBuilder&) -> RegistryBuilder& = delete;
    auto operator=(RegistryBuilder&&) -> RegistryBuilder&      = default;


    [[nodiscard]]
    auto get_allocator() const noexcept -> allocator_type;

    template <entry_c Entry_T, typename Self_T>
    auto register_entry(this Self_T&&) -> Self_T&&;

    [[nodiscard]]
    auto build(
        std::pmr::memory_resource& transient_memory_resource
        = *std::pmr::get_default_resource()
    ) && -> Registry;

private:
    EntryInjectionContainer m_injections;
    EntryBuilderContainer   m_builders;
    Registry                m_registry;
};

}   // namespace redi

namespace redi {

template <typename Entry_T>
concept buildable_entry_c = std::derived_from<Entry_T, internal::BuildableEntryBase>;

template <decays_to_entry_c... Entries_T>
RegistryBuilder::RegistryBuilder(
    std::allocator_arg_t,
    const allocator_type& allocator,
    Entries_T&&... entries
)
    : RegistryBuilder{ allocator }
{
    (m_registry.insert(std::forward<Entries_T>(entries)), ...);
}

template <entry_c Entry_T, typename Self_T>
auto RegistryBuilder::register_entry(this Self_T&& self) -> Self_T&&
{
    if constexpr (buildable_entry_c<Entry_T>)
    {
        BuildDirector<Entry_T> build_director{
            self.RegistryBuilder::m_injections,
            self.RegistryBuilder::m_builders,
            self.RegistryBuilder::m_registry,
        };

        describe_build(std::type_identity<internal::BuildableEntryBase>{}, build_director);
    }
    else
    {
        self.RegistryBuilder::m_registry.template try_emplace<Entry_T>();
    }

    return std::forward<Self_T>(self);
}

}   // namespace redi
