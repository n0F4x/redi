#include <concepts>
#include <functional>
#include <memory_resource>
#include <string>
#include <utility>

#include <catch2/catch_test_macros.hpp>

import redi.BuildDirector;
import redi.BuiltEntryBuilderTag;
import redi.CyclicDependencyDetected;
import redi.entry_builder_c;
import redi.entry_c;
import redi.EntryBuilderTag;
import redi.EntryTraits;
import redi.Registry;
import redi.RegistryBuilder;
import redi.util.containers.OptionalRef;
import redi.util.reflection;

namespace redi {

namespace {

const std::string type_name{ util::name_of<RegistryBuilder>() };

template <typename Builder_T>
struct BuilderBuildDescriber {
    friend auto describe_build(BuildDirector<Builder_T>& build_director) -> void
    {
        build_director.template use_function<Builder_T::create>();
    }
};

struct BuiltEntry {};

struct ConfigurationEntry {};

struct SelfBuildDependentEntry : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(SelfBuildDependentEntry&) noexcept
            -> SelfBuildDependentEntry
        {
            return SelfBuildDependentEntry{};
        }
    };
};

struct SelfCreateDependentEntry : BuiltEntry {
    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto create(Builder&) noexcept -> Builder
        {
            return Builder{};
        }

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> SelfCreateDependentEntry
        {
            return SelfCreateDependentEntry{};
        }
    };
};

struct OptionalSelfBuildDependentEntry : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            util::OptionalRef<OptionalSelfBuildDependentEntry>
        ) noexcept -> OptionalSelfBuildDependentEntry
        {
            return OptionalSelfBuildDependentEntry{};
        }
    };
};

struct OptionalSelfCreateDependentEntry : BuiltEntry {
    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto create(util::OptionalRef<Builder>) noexcept -> Builder
        {
            return Builder{};
        }

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> OptionalSelfCreateDependentEntry
        {
            return OptionalSelfCreateDependentEntry{};
        }
    };
};

struct EntryCyclicBuildEntryA;
struct EntryCyclicBuildEntryB;

struct EntryCyclicBuildEntryA : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryCyclicBuildEntryB&) noexcept
            -> EntryCyclicBuildEntryA
        {
            return EntryCyclicBuildEntryA{};
        }
    };
};

struct EntryCyclicBuildEntryB : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryCyclicBuildEntryA&) noexcept
            -> EntryCyclicBuildEntryB
        {
            return EntryCyclicBuildEntryB{};
        }
    };
};

struct BuilderCyclicBuildEntryA : BuiltEntry {
    struct Builder;
};

struct BuilderCyclicBuildEntryB : BuiltEntry {
    struct Builder;
};

struct EntryAndBuilderCyclicBuildEntryA : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> EntryAndBuilderCyclicBuildEntryA
        {
            return EntryAndBuilderCyclicBuildEntryA{};
        }
    };
};

struct EntryAndBuilderCyclicBuildEntryB : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            EntryAndBuilderCyclicBuildEntryA&,
            const EntryAndBuilderCyclicBuildEntryA::Builder&
        ) noexcept -> EntryAndBuilderCyclicBuildEntryB
        {
            return EntryAndBuilderCyclicBuildEntryB{};
        }
    };
};

struct BuilderCyclicBuildEntryA::Builder {
    // ReSharper disable once CppTypeAliasNeverUsed
    using BuilderConcept = EntryBuilderTag;

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const BuilderCyclicBuildEntryB::Builder&) noexcept
        -> BuilderCyclicBuildEntryA
    {
        return BuilderCyclicBuildEntryA{};
    }
};

struct BuilderCyclicBuildEntryB::Builder {
    // ReSharper disable once CppTypeAliasNeverUsed
    using BuilderConcept = EntryBuilderTag;

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const BuilderCyclicBuildEntryA::Builder&) noexcept
        -> BuilderCyclicBuildEntryB
    {
        return BuilderCyclicBuildEntryB{};
    }
};

struct CyclicCreateEntryA : BuiltEntry {
    struct Builder;
};

struct CyclicCreateEntryB : BuiltEntry {
    struct Builder;
};

struct CyclicCreateEntryA::Builder : BuilderBuildDescriber<Builder> {
    // ReSharper disable once CppTypeAliasNeverUsed
    using BuilderConcept = BuiltEntryBuilderTag;

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto create(CyclicCreateEntryB::Builder&) noexcept -> Builder
    {
        return Builder{};
    }

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build() noexcept -> CyclicCreateEntryA
    {
        return CyclicCreateEntryA{};
    }
};

struct CyclicCreateEntryB::Builder : BuilderBuildDescriber<Builder> {
    // ReSharper disable once CppTypeAliasNeverUsed
    using BuilderConcept = BuiltEntryBuilderTag;

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto create(CyclicCreateEntryA::Builder&) noexcept -> Builder
    {
        return Builder{};
    }

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build() noexcept -> CyclicCreateEntryB
    {
        return CyclicCreateEntryB{};
    }
};

struct ConflictingMixedDependenciesEntryA : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> ConflictingMixedDependenciesEntryA
        {
            return ConflictingMixedDependenciesEntryA{};
        }
    };
};

struct ConflictingMixedDependenciesEntryB : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> ConflictingMixedDependenciesEntryB
        {
            return ConflictingMixedDependenciesEntryB{};
        }
    };
};

struct ConflictingMixedDependenciesEntryC : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            ConflictingMixedDependenciesEntryB&,
            const ConflictingMixedDependenciesEntryA::Builder&
        ) noexcept -> ConflictingMixedDependenciesEntryC
        {
            return ConflictingMixedDependenciesEntryC{};
        }
    };
};

struct ConflictingMixedDependenciesEntryD : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            ConflictingMixedDependenciesEntryA&,
            const ConflictingMixedDependenciesEntryB::Builder&
        ) noexcept -> ConflictingMixedDependenciesEntryD
        {
            return ConflictingMixedDependenciesEntryD{};
        }
    };
};

struct EntryDependencyA {};

struct EntryDependencyB : BuiltEntry {
    constexpr explicit EntryDependencyB(EntryDependencyA&) noexcept {}

    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryDependencyA& a) noexcept -> EntryDependencyB
        {
            return EntryDependencyB{ a };
        }
    };
};

struct EntryBuilderDependencyA : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> EntryBuilderDependencyA
        {
            return EntryBuilderDependencyA{};
        }
    };
};

struct EntryBuilderDependencyB : BuiltEntry {
    constexpr explicit EntryBuilderDependencyB(
        const EntryBuilderDependencyA::Builder&
    ) noexcept
    {
    }

    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            const EntryBuilderDependencyA::Builder& a_builder
        ) noexcept -> EntryBuilderDependencyB
        {
            return EntryBuilderDependencyB{ a_builder };
        }
    };
};

struct BuilderEntryDependencyA : ConfigurationEntry {};

struct BuilderEntryDependencyB : BuiltEntry {
    constexpr explicit BuilderEntryDependencyB(BuilderEntryDependencyA&) noexcept {}

    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto create(BuilderEntryDependencyA& a) noexcept -> Builder
        {
            return Builder{ a };
        }

        std::reference_wrapper<BuilderEntryDependencyA> a;

        // ReSharper disable once CppDFAUnreachableFunctionCall
        constexpr explicit Builder(BuilderEntryDependencyA& a) : a{ a } {}

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr auto build() const noexcept -> BuilderEntryDependencyB
        {
            return BuilderEntryDependencyB{ a };
        }
    };
};

struct DependencyInversionEntryA : BuiltEntry {
    struct Builder;
};

struct DependencyInversionEntryB : BuiltEntry {
    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        [[nodiscard]]
        constexpr static auto create(DependencyInversionEntryA::Builder&) noexcept
            -> Builder
        {
            return Builder{};
        }

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(DependencyInversionEntryA&) noexcept
            -> DependencyInversionEntryB
        {
            return DependencyInversionEntryB{};
        }
    };
};

struct DependencyInversionEntryA::Builder {
    // ReSharper disable once CppTypeAliasNeverUsed
    using BuilderConcept = EntryBuilderTag;

    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const DependencyInversionEntryB::Builder&) noexcept
        -> DependencyInversionEntryA
    {
        return DependencyInversionEntryA{};
    }
};

struct EntryAndBuilderSharedDependencyEntryA : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> EntryAndBuilderSharedDependencyEntryA
        {
            return EntryAndBuilderSharedDependencyEntryA{};
        }
    };
};

struct EntryAndBuilderSharedDependencyEntryB : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryAndBuilderSharedDependencyEntryA&) noexcept
            -> EntryAndBuilderSharedDependencyEntryB
        {
            return EntryAndBuilderSharedDependencyEntryB{};
        }
    };
};

struct EntryAndBuilderSharedDependencyEntryC : BuiltEntry {
    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(
            const EntryAndBuilderSharedDependencyEntryA::Builder&
        ) noexcept -> EntryAndBuilderSharedDependencyEntryC
        {
            return EntryAndBuilderSharedDependencyEntryC{};
        }
    };
};

struct BuilderOptionalEntryDependencyA : ConfigurationEntry {};

struct BuilderOptionalEntryDependencyB : BuiltEntry {
    bool has_dependency;

    constexpr explicit BuilderOptionalEntryDependencyB(const bool has_dependency) noexcept
        : has_dependency{ has_dependency }
    {
    }

    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto create(
            const util::OptionalRef<const BuilderOptionalEntryDependencyA> a
        ) noexcept -> Builder
        {
            return Builder{ a.has_value() };
        }

        bool has_dependency;

        // ReSharper disable once CppDFAUnreachableFunctionCall
        constexpr explicit Builder(const bool has_dependency) noexcept
            : has_dependency{ has_dependency }
        {
        }

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr auto build() const noexcept -> BuilderOptionalEntryDependencyB
        {
            return BuilderOptionalEntryDependencyB{ has_dependency };
        }
    };
};

struct ImmovableEntryDependency {};

struct ImmovableBuiltEntry : BuiltEntry {
    ImmovableEntryDependency& dependency;

    constexpr explicit ImmovableBuiltEntry(ImmovableEntryDependency& dependency) noexcept
        : dependency{ dependency }
    {
    }

    ImmovableBuiltEntry(const ImmovableBuiltEntry&) = delete;
    ImmovableBuiltEntry(ImmovableBuiltEntry&&)      = delete;

    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(ImmovableEntryDependency& dependency) noexcept
            -> ImmovableBuiltEntry
        {
            return ImmovableBuiltEntry{ dependency };
        }
    };
};

struct ImmovableDependenciesEntry {
    ImmovableEntryDependency& dependency;

    constexpr explicit ImmovableDependenciesEntry(
        ImmovableEntryDependency& dependency
    ) noexcept
        : dependency{ dependency }
    {
    }

    ImmovableDependenciesEntry(const ImmovableDependenciesEntry&) = delete;
    ImmovableDependenciesEntry(ImmovableDependenciesEntry&&)      = delete;
};

struct ImmovableBuilderEntry : BuiltEntry {
    struct Builder : BuilderBuildDescriber<Builder> {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = BuiltEntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto create() noexcept -> Builder
        {
            return Builder{};
        }

        constexpr Builder() noexcept = default;

        Builder(const Builder&) = delete;
        Builder(Builder&&)      = delete;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> ImmovableBuilderEntry
        {
            return ImmovableBuilderEntry{};
        }
    };
};

struct AllocatorAwareBuiltEntry : BuiltEntry {
    using allocator_type = std::pmr::polymorphic_allocator<>;

    struct Builder {
        // ReSharper disable once CppTypeAliasNeverUsed
        using BuilderConcept = EntryBuilderTag;

        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        static auto build() -> AllocatorAwareBuiltEntry
        {
            return AllocatorAwareBuiltEntry{};
        }
    };

    explicit AllocatorAwareBuiltEntry(const allocator_type& allocator = {})
        : m_allocator{ allocator }
    {
    }

    AllocatorAwareBuiltEntry(AllocatorAwareBuiltEntry&&) noexcept = default;

    AllocatorAwareBuiltEntry(AllocatorAwareBuiltEntry&&, const allocator_type& allocator)
        : m_allocator{ allocator }
    {
    }

    [[nodiscard]]
    auto get_allocator() const noexcept -> allocator_type
    {
        return m_allocator;
    }

private:
    allocator_type m_allocator;
};

}   // namespace

template <std::derived_from<BuiltEntry> Entry_T>
    requires redi::entry_c<Entry_T>
struct EntryTraits<Entry_T> {
    constexpr static auto describe_build(BuildDirector<Entry_T>& build_director) -> void
    {
        build_director.template use_builder<typename Entry_T::Builder>();
    }
};

template <std::derived_from<ConfigurationEntry> Entry_T>
    requires redi::entry_c<Entry_T>
struct EntryTraits<Entry_T> {
    constexpr static bool is_configuration_entry{ true };
};

template <>
struct EntryTraits<ImmovableDependenciesEntry> {
    constexpr static auto describe_build(
        BuildDirector<ImmovableDependenciesEntry>& build_director
    ) -> void
    {
        build_director.use_dependencies<ImmovableEntryDependency&>();
    }
};

TEST_CASE(type_name)
{
    std::pmr::unsynchronized_pool_resource transient_memory_resource{
        std::pmr::get_default_resource()
    };

#ifdef REDI_TEST
    SECTION("cyclic dependency detection")
    {
        SECTION("self")
        {
            SECTION("build")
            {
                RegistryBuilder registry_builder;

                registry_builder.register_entry<SelfBuildDependentEntry>();

                REQUIRE_THROWS_AS(
                    std::move(registry_builder).build(transient_memory_resource),
                    CyclicDependencyDetected
                );
            }

            SECTION("create")
            {
                RegistryBuilder registry_builder;

                registry_builder.register_entry<SelfCreateDependentEntry>();

                REQUIRE_THROWS_AS(
                    std::move(registry_builder).build(transient_memory_resource),
                    CyclicDependencyDetected
                );
            }
        }

        SECTION("optional self")
        {
            SECTION("build")
            {
                RegistryBuilder registry_builder;

                registry_builder.register_entry<OptionalSelfBuildDependentEntry>();

                REQUIRE_THROWS_AS(
                    std::move(registry_builder).build(transient_memory_resource),
                    CyclicDependencyDetected
                );
            }

            SECTION("create")
            {
                RegistryBuilder registry_builder;

                registry_builder.register_entry<OptionalSelfCreateDependentEntry>();

                REQUIRE_THROWS_AS(
                    std::move(registry_builder).build(transient_memory_resource),
                    CyclicDependencyDetected
                );
            }
        }

        SECTION("entry -> entry cycle")
        {
            RegistryBuilder registry_builder;

            registry_builder.register_entry<EntryCyclicBuildEntryA>();

            REQUIRE_THROWS_AS(
                std::move(registry_builder).build(transient_memory_resource),
                CyclicDependencyDetected
            );
        }

        SECTION("entry -> builder cycle")
        {
            RegistryBuilder registry_builder;

            registry_builder.register_entry<BuilderCyclicBuildEntryA>();

            REQUIRE_THROWS_AS(
                std::move(registry_builder).build(transient_memory_resource),
                CyclicDependencyDetected
            );
        }

        SECTION("entry -> (entry, builder)")
        {
            RegistryBuilder registry_builder;

            registry_builder.register_entry<EntryAndBuilderCyclicBuildEntryB>();

            REQUIRE_THROWS_AS(
                std::move(registry_builder).build(transient_memory_resource),
                CyclicDependencyDetected
            );
        }

        SECTION("builder -> builder cycle")
        {
            RegistryBuilder registry_builder;

            registry_builder.register_entry<CyclicCreateEntryA>();

            REQUIRE_THROWS_AS(
                std::move(registry_builder).build(transient_memory_resource),
                CyclicDependencyDetected
            );
        }

        SECTION("conflicting entry and builder dependencies across builders")
        {
            RegistryBuilder registry_builder;

            registry_builder.register_entry<ConflictingMixedDependenciesEntryC>();
            registry_builder.register_entry<ConflictingMixedDependenciesEntryD>();

            REQUIRE_THROWS_AS(
                std::move(registry_builder).build(transient_memory_resource),
                CyclicDependencyDetected
            );
        }
    }
#endif

    SECTION("entry -> entry dependency")
    {
        SECTION("base")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryDependencyA>();
            registry_builder.register_entry<EntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryDependencyA>());
            REQUIRE(registry.contains<EntryDependencyB>());
        }

        SECTION("reordered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryDependencyB>();
            registry_builder.register_entry<EntryDependencyA>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryDependencyA>());
            REQUIRE(registry.contains<EntryDependencyB>());
        }

        SECTION("automatically registered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryDependencyA>());
            REQUIRE(registry.contains<EntryDependencyB>());
        }
    }

    SECTION("entry -> builder dependency")
    {
        SECTION("base")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryBuilderDependencyA>();
            registry_builder.register_entry<EntryBuilderDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryBuilderDependencyA>());
            REQUIRE(registry.contains<EntryBuilderDependencyB>());
        }

        SECTION("reordered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryBuilderDependencyB>();
            registry_builder.register_entry<EntryBuilderDependencyA>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryBuilderDependencyA>());
            REQUIRE(registry.contains<EntryBuilderDependencyB>());
        }

        SECTION("automatically registered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryBuilderDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryBuilderDependencyA>());
            REQUIRE(registry.contains<EntryBuilderDependencyB>());
        }
    }

    SECTION("builder -> (configuration) entry dependency")
    {
        SECTION("base")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderEntryDependencyA>();
            registry_builder.register_entry<BuilderEntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<BuilderEntryDependencyA>());
            REQUIRE(registry.contains<BuilderEntryDependencyB>());
        }

        SECTION("reordered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderEntryDependencyB>();
            registry_builder.register_entry<BuilderEntryDependencyA>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<BuilderEntryDependencyA>());
            REQUIRE(registry.contains<BuilderEntryDependencyB>());
        }

        SECTION("automatically registered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderEntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<BuilderEntryDependencyA>());
            REQUIRE(registry.contains<BuilderEntryDependencyB>());
        }
    }

    SECTION("builder -> optional (configuration) entry dependency")
    {
        SECTION("present")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderOptionalEntryDependencyA>();
            registry_builder.register_entry<BuilderOptionalEntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<BuilderOptionalEntryDependencyA>());
            REQUIRE(registry.at<BuilderOptionalEntryDependencyB>().has_dependency);
        }

        SECTION("reordered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderOptionalEntryDependencyB>();
            registry_builder.register_entry<BuilderOptionalEntryDependencyA>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<BuilderOptionalEntryDependencyA>());
            REQUIRE(registry.at<BuilderOptionalEntryDependencyB>().has_dependency);
        }

        SECTION("absent (not registered automatically)")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<BuilderOptionalEntryDependencyB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE_FALSE(registry.contains<BuilderOptionalEntryDependencyA>());
            REQUIRE_FALSE(registry.at<BuilderOptionalEntryDependencyB>().has_dependency);
        }
    }

    SECTION("entryB -> entryA -> builderB -> builderA")
    {
        SECTION("base")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<DependencyInversionEntryA>();
            registry_builder.register_entry<DependencyInversionEntryB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<DependencyInversionEntryA>());
            REQUIRE(registry.contains<DependencyInversionEntryB>());
        }
    }

    SECTION("entryB -> entryA; entryC -> builderA")
    {
        SECTION("base")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryAndBuilderSharedDependencyEntryB>();
            registry_builder.register_entry<EntryAndBuilderSharedDependencyEntryC>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryB>());
            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryA>());
            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryC>());
        }

        SECTION("reordered")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<EntryAndBuilderSharedDependencyEntryC>();
            registry_builder.register_entry<EntryAndBuilderSharedDependencyEntryB>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryB>());
            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryA>());
            REQUIRE(registry.contains<EntryAndBuilderSharedDependencyEntryC>());
        }
    }

    SECTION("immovable entries")
    {
        SECTION("built by a builder")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<ImmovableBuiltEntry>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(
                &registry.at<ImmovableBuiltEntry>().dependency
                == &registry.at<ImmovableEntryDependency>()
            );
        }

        SECTION("built from dependencies")
        {
            RegistryBuilder registry_builder;
            registry_builder.register_entry<ImmovableDependenciesEntry>();
            Registry registry
                = std::move(registry_builder).build(transient_memory_resource);

            REQUIRE(
                &registry.at<ImmovableDependenciesEntry>().dependency
                == &registry.at<ImmovableEntryDependency>()
            );
        }
    }

    SECTION("immovable built builder")
    {
        RegistryBuilder registry_builder;
        registry_builder.register_entry<ImmovableBuilderEntry>();
        Registry registry = std::move(registry_builder).build(transient_memory_resource);

        REQUIRE(registry.contains<ImmovableBuilderEntry>());
    }

    SECTION("allocator-aware entries use the allocator of the registry")
    {
        std::pmr::unsynchronized_pool_resource registry_memory_resource;

        RegistryBuilder registry_builder{
            RegistryBuilder::allocator_type{ &registry_memory_resource },
        };
        registry_builder.register_entry<AllocatorAwareBuiltEntry>();
        Registry registry = std::move(registry_builder).build(transient_memory_resource);

        REQUIRE(
            registry.at<AllocatorAwareBuiltEntry>().get_allocator().resource()
            == &registry_memory_resource
        );
    }
}

}   // namespace redi
