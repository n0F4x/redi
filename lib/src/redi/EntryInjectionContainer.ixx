module;

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <functional>
#include <memory_resource>
#include <optional>
#include <string>
#include <vector>

export module redi.EntryInjectionContainer;

import redi.ConfigurationEntry;
import redi.DependencyChainNode;
import redi.EntryBuilderBase;
import redi.EntryBuilderContainer;
import redi.strip_dependency_t;
import redi.Registry;
import redi.util.concepts.function;
import redi.util.concepts.function_pointer;
import redi.util.containers.MoveOnlyFunction;
import redi.util.containers.OptionalRef;
import redi.util.reflection;
import redi.util.type_traits.arguments_of;
import redi.util.type_traits.forward_like;
import redi.util.type_traits.result_of;
import redi.util.TypeList;

namespace redi {

using ErasedEntryInjection
    = util::MoveOnlyFunction<auto(EntryBuilderContainer&, Registry&) &&->void, 0>;

export class EntryInjectionContainer {
public:
    using allocator_type = std::pmr::polymorphic_allocator<>;


    EntryInjectionContainer(EntryInjectionContainer&&, const allocator_type&);

    explicit EntryInjectionContainer() = default;
    explicit EntryInjectionContainer(const allocator_type&);


    [[nodiscard]]
    auto get_allocator() const noexcept -> allocator_type;

    template <util::function_c Injection_T>
    [[nodiscard]]
    auto contains() const noexcept -> bool;


    template <auto injection_T>
    auto try_insert() -> bool;

    auto build(
        EntryBuilderContainer&     builders,
        Registry&                  registry,
        std::pmr::memory_resource& transient_memory_resource
    ) && -> void;

private:
    std::pmr::vector<ErasedEntryInjection>       m_injections;
    std::pmr::vector<uint64_t>                   m_builder_hashes;
    std::pmr::vector<std::pmr::vector<uint64_t>> m_dependency_hashes;
    std::pmr::vector<std::optional<std::pmr::vector<uint64_t>>> m_dependent_builder_hashes;
#ifdef REDI_DEBUG
    std::pmr::vector<std::pmr::string> m_builder_names;
#endif


    auto sort() -> void;

    [[nodiscard]]
    auto check_cyclic_dependencies(
        std::pmr::memory_resource& transient_memory_resource
    ) const -> bool;

    auto collect_dependent_builder_hashes() -> void;
    auto bubble_up_dependencies_of(std::size_t index) -> void;


    [[nodiscard]]
    auto check_cyclic_dependencies(
        std::size_t                 injection_index,
        const DependencyChainNode&  dependency_chain,
        std::pmr::vector<uint64_t>& hash_cache,
        std::pmr::memory_resource&  transient_memory_resource
    ) const -> bool;
};

}   // namespace redi

namespace redi {

template <auto injection_T>
struct ErasedEntryInjectionLambda {
    static auto operator()(EntryBuilderContainer& builders, Registry& registry) -> void
    {
        helper(injection_T, builders, registry);
    }

    template <typename... Dependencies_T, typename Builder_T>
    static auto helper(
        auto (*)(Dependencies_T...)->Builder_T,
        EntryBuilderContainer& builders,
        Registry&              registry
    ) -> void
    {
        builders.insert(
            std::invoke(
                injection_T,
                fetch_dependency<Dependencies_T>(builders, registry)...
            )
        );
    }

    template <typename Dependency_T>
    // ReSharper disable once CppNotAllPathsReturnValue
    static auto fetch_dependency(EntryBuilderContainer& builders, Registry& registry)
        -> Dependency_T
    {
        using StrippedDependency = strip_dependency_t<Dependency_T>;

        if constexpr (std::derived_from<StrippedDependency, EntryBuilderBase>)
        {
            if constexpr (util::optional_ref_c<Dependency_T>)
            {
                return builders.find<StrippedDependency>();
            }
            else
            {
                return builders.at<StrippedDependency>();
            }
        }
        else if constexpr (std::derived_from<StrippedDependency, ConfigurationEntry>)
        {
            if constexpr (util::optional_ref_c<Dependency_T>)
            {
                return registry.find<StrippedDependency>();
            }
            else
            {
                return registry.at<StrippedDependency>();
            }
        }
        else
        {
            static_assert(false, "invalid dependency");
        }
    }
};

template <util::function_c Injection_T>
auto EntryInjectionContainer::contains() const noexcept -> bool
{
    return std::ranges::contains(
        m_builder_hashes,
        util::hash_u64<util::result_of_t<Injection_T>>()
    );
}

template <auto injection_T>
auto EntryInjectionContainer::try_insert() -> bool
{
    using Builder = util::result_of_t<decltype(injection_T)>;

    if (contains<std::remove_pointer_t<decltype(injection_T)>>())
    {
        return false;
    }

    m_injections.emplace_back(ErasedEntryInjectionLambda<injection_T>{});
    m_builder_hashes.push_back(util::hash_u64<Builder>());
    [this]<typename... Dependencies_T>(util::TypeList<Dependencies_T...>) -> void
    {
        m_dependency_hashes.emplace_back(
            std::initializer_list{
                util::hash_u64<strip_dependency_t<Dependencies_T>>()... }
        );
    }(util::arguments_of_t<decltype(injection_T)>{});
    m_dependent_builder_hashes.emplace_back();

#ifdef REDI_DEBUG
    m_builder_names.emplace_back(util::name_of<Builder>());
#endif

    return true;
}

}   // namespace redi
