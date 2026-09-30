#include <functional>
#include <memory_resource>

#include <catch2/catch_test_macros.hpp>

import redi.BuildableEntryBuilder;
import redi.BuildDirector;
import redi.CyclicDependencyDetected;
import redi.entry_c;
import redi.EntryBuilderBase;
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
    constexpr static auto operator()(BuildDirector<Builder_T>& build_director) -> void
    {
        build_director.template use_function<Builder_T::create>();
    }
};

struct BuildableEntry {};

struct ConfigurationEntry {};

struct SelfBuildDependentEntry : BuildableEntry {
    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(SelfBuildDependentEntry&) noexcept
            -> SelfBuildDependentEntry
        {
            return SelfBuildDependentEntry{};
        }
    };
};

struct SelfCreateDependentEntry : BuildableEntry {
    struct Builder : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct OptionalSelfBuildDependentEntry : BuildableEntry {
    struct Builder : EntryBuilderBase {
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

struct OptionalSelfCreateDependentEntry : BuildableEntry {
    struct Builder : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct EntryCyclicBuildEntryA : BuildableEntry {
    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryCyclicBuildEntryB&) noexcept
            -> EntryCyclicBuildEntryA
        {
            return EntryCyclicBuildEntryA{};
        }
    };
};

struct EntryCyclicBuildEntryB : BuildableEntry {
    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryCyclicBuildEntryA&) noexcept
            -> EntryCyclicBuildEntryB
        {
            return EntryCyclicBuildEntryB{};
        }
    };
};

struct BuilderCyclicBuildEntryA : BuildableEntry {
    struct Builder;
};

struct BuilderCyclicBuildEntryB : BuildableEntry {
    struct Builder;
};

struct EntryAndBuilderCyclicBuildEntryA : BuildableEntry {
    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> EntryAndBuilderCyclicBuildEntryA
        {
            return EntryAndBuilderCyclicBuildEntryA{};
        }
    };
};

struct EntryAndBuilderCyclicBuildEntryB : BuildableEntry {
    struct Builder : EntryBuilderBase {
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

struct BuilderCyclicBuildEntryA::Builder : EntryBuilderBase {
    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const BuilderCyclicBuildEntryB::Builder&) noexcept
        -> BuilderCyclicBuildEntryA
    {
        return BuilderCyclicBuildEntryA{};
    }
};

struct BuilderCyclicBuildEntryB::Builder : EntryBuilderBase {
    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const BuilderCyclicBuildEntryA::Builder&) noexcept
        -> BuilderCyclicBuildEntryB
    {
        return BuilderCyclicBuildEntryB{};
    }
};

struct CyclicCreateEntryA : BuildableEntry {
    struct Builder;
};

struct CyclicCreateEntryB : BuildableEntry {
    struct Builder;
};

struct CyclicCreateEntryA::Builder
    : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct CyclicCreateEntryB::Builder
    : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct EntryDependencyA {};

struct EntryDependencyB : BuildableEntry {
    constexpr explicit EntryDependencyB(EntryDependencyA&) noexcept {}

    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build(EntryDependencyA& a) noexcept -> EntryDependencyB
        {
            return EntryDependencyB{ a };
        }
    };
};

struct EntryBuilderDependencyA : BuildableEntry {
    struct Builder : EntryBuilderBase {
        // ReSharper disable once CppDeclaratorNeverUsed
        [[nodiscard]]
        constexpr static auto build() noexcept -> EntryBuilderDependencyA
        {
            return EntryBuilderDependencyA{};
        }
    };
};

struct EntryBuilderDependencyB : BuildableEntry {
    constexpr explicit EntryBuilderDependencyB(
        const EntryBuilderDependencyA::Builder&
    ) noexcept
    {
    }

    struct Builder : EntryBuilderBase {
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

struct BuilderEntryDependencyB : BuildableEntry {
    constexpr explicit BuilderEntryDependencyB(BuilderEntryDependencyA&) noexcept {}

    struct Builder : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct DependencyInversionEntryA : BuildableEntry {
    struct Builder;
};

struct DependencyInversionEntryB : BuildableEntry {
    struct Builder : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

struct DependencyInversionEntryA::Builder : EntryBuilderBase {
    // ReSharper disable once CppDeclaratorNeverUsed
    [[nodiscard]]
    constexpr static auto build(const DependencyInversionEntryB::Builder&) noexcept
        -> DependencyInversionEntryA
    {
        return DependencyInversionEntryA{};
    }
};

struct BuilderOptionalEntryDependencyA : ConfigurationEntry {};

struct BuilderOptionalEntryDependencyB : BuildableEntry {
    bool has_dependency;

    constexpr explicit BuilderOptionalEntryDependencyB(const bool has_dependency) noexcept
        : has_dependency{ has_dependency }
    {
    }

    struct Builder : BuildableEntryBuilder<Builder, BuilderBuildDescriber<Builder>{}> {
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

}   // namespace

template <std::derived_from<BuildableEntry> Entry_T>
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

TEST_CASE(type_name)
{
    std::pmr::unsynchronized_pool_resource transient_memory_resource{
        std::pmr::get_default_resource()
    };

#ifdef REDI_DEBUG
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

    SECTION("entryB -> entryA -> builderB -> builderA)")
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
}

}   // namespace redi
