module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory_resource>
#include <numeric>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "redi/util/contract_macros.hpp"

module redi.EntryBuilderContainer;

import redi.CyclicDependencyDetected;
import redi.util.contracts;

namespace redi {

EntryBuilderContainer::EntryBuilderContainer(
    EntryBuilderContainer&& other,
    const allocator_type&   allocator
)
    : m_builders{ std::move(other.m_builders), allocator },
      m_entry_hashes{ std::move(other.m_entry_hashes), allocator },
      m_builder_dependency_entry_hashes{
          std::move(other.m_builder_dependency_entry_hashes),
          allocator,
      },
      m_entry_dependency_hashes{ std::move(other.m_entry_dependency_hashes), allocator }
#ifdef REDI_DEBUG
      ,
      m_entry_names{ std::move(other.m_entry_names), allocator },
      m_builder_names{ std::move(other.m_builder_names), allocator }
#endif
{
}

EntryBuilderContainer::EntryBuilderContainer(const allocator_type& allocator)
    : m_builders{ allocator },
      m_entry_hashes{ allocator },
      m_builder_dependency_entry_hashes{ allocator },
      m_entry_dependency_hashes{ allocator }
#ifdef REDI_DEBUG
      ,
      m_entry_names{ allocator },
      m_builder_names{ allocator }
#endif
{
}

auto EntryBuilderContainer::get_allocator() const noexcept -> allocator_type
{
    return m_builders.get_allocator();
}

struct EntryBuilderContainer::BuildOrderGraph {
    std::pmr::vector<std::size_t> node_edge_offsets;
    std::pmr::vector<std::size_t> edge_target_indices;
    std::pmr::vector<std::size_t> in_degrees;

    [[nodiscard]]
    auto target_indices_of(const std::size_t index) const noexcept
        -> std::span<const std::size_t>
    {
        return std::span{ edge_target_indices }.subspan(
            node_edge_offsets[index],
            node_edge_offsets[index + 1] - node_edge_offsets[index]
        );
    }
};

auto EntryBuilderContainer::build(
    Registry&                  registry,
    std::pmr::memory_resource& transient_memory_resource
) && -> void
{
    BuildOrderGraph graph{
        make_build_order_graph(&transient_memory_resource, transient_memory_resource)
    };

    PRECOND(!check_cyclic_dependencies(graph, transient_memory_resource));

    for (const std::size_t index : sort_topologically(graph, transient_memory_resource))
    {
        std::move(m_builders[index])(*this, registry);
    }
}

auto EntryBuilderContainer::try_index_of_builder(const uint64_t hash) const noexcept
    -> std::optional<std::size_t>
{
    const auto hash_iter{ std::ranges::find(m_entry_hashes, hash) };
    if (hash_iter == m_entry_hashes.cend())
    {
        return std::nullopt;
    }
    return static_cast<std::size_t>(std::distance(m_entry_hashes.begin(), hash_iter));
}

auto EntryBuilderContainer::make_build_order_graph(
    const std::pmr::polymorphic_allocator<>& allocator,
    std::pmr::memory_resource&               transient_memory_resource
) const -> BuildOrderGraph
{
    const std::size_t builder_count{ m_builders.size() };

    BuildOrderGraph graph{
        .node_edge_offsets{ builder_count + 1, 0, allocator },
        .edge_target_indices{ allocator },
        .in_degrees{ builder_count, 0, allocator },
    };

    const auto for_each_edge{
        [this, builder_count](auto&& visit) -> void
        {
            for (std::size_t index{}; index < builder_count; ++index)
            {
                for (const uint64_t dependency_hash : m_entry_dependency_hashes[index])
                {
                    if (const std::optional dependency_index{
                            try_index_of_builder(dependency_hash),
                        };
                        dependency_index.has_value())
                    {
                        visit(*dependency_index, index);
                    }
                }

                for (
                    const uint64_t dependency_hash :
                    m_builder_dependency_entry_hashes[index]
                )
                {
                    if (const std::optional dependency_index{
                            try_index_of_builder(dependency_hash),
                        };
                        dependency_index.has_value())
                    {
                        visit(index, *dependency_index);
                    }
                }
            }
        },
    };

    for_each_edge(
        [&graph](const std::size_t from, const std::size_t to) noexcept -> void
        {
            ++graph.node_edge_offsets[from + 1];
            ++graph.in_degrees[to];
        }
    );

    std::inclusive_scan(
        graph.node_edge_offsets.begin(),
        graph.node_edge_offsets.end(),
        graph.node_edge_offsets.begin()
    );
    graph.edge_target_indices.resize(graph.node_edge_offsets.back());

    std::pmr::vector<std::size_t> target_tracker{
        graph.node_edge_offsets.begin(),
        std::ranges::prev(graph.node_edge_offsets.end()),
        &transient_memory_resource,
    };
    for_each_edge(
        [&graph, &target_tracker]   //
        (const std::size_t from, const std::size_t to) noexcept -> void
        {
            graph.edge_target_indices[target_tracker[from]] = to;
            ++target_tracker[from];
        }
    );

    return graph;
}

auto EntryBuilderContainer::sort_topologically(
    BuildOrderGraph&           graph,
    std::pmr::memory_resource& transient_memory_resource
) -> std::pmr::vector<std::size_t>
{
    /*
     * Kahn's algorithm
     * Sorting requires that there is no cyclic dependency,
     *  otherwise the builders taking part in a cycle are left out
     */
    std::pmr::vector<std::size_t> build_order{ &transient_memory_resource };
    build_order.reserve(graph.in_degrees.size());

    for (std::size_t index{}; index < graph.in_degrees.size(); ++index)
    {
        if (graph.in_degrees[index] == 0)
        {
            build_order.push_back(index);
        }
    }

    for (std::size_t head_index{}; head_index < build_order.size(); ++head_index)
    {
        for (
            const std::size_t target_index :
            graph.target_indices_of(build_order[head_index])
        )
        {
            --graph.in_degrees[target_index];
            if (graph.in_degrees[target_index] == 0)
            {
                build_order.push_back(target_index);
            }
        }
    }

    return build_order;
}

#ifdef REDI_DEBUG
auto EntryBuilderContainer::check_cyclic_dependencies(
    const BuildOrderGraph&     graph,
    std::pmr::memory_resource& transient_memory_resource
) const -> bool
{
    enum struct VisitState : uint8_t {
        eUnvisited,
        eInProgress,
        eDone,
    };

    const std::size_t builder_count{ m_builders.size() };

    std::pmr::vector<VisitState> visit_states{
        builder_count,
        VisitState::eUnvisited,
        &transient_memory_resource,
    };
    std::pmr::vector<std::size_t> path{ &transient_memory_resource };
    std::pmr::vector<std::size_t> next_edge_indices{ &transient_memory_resource };

    /*
     * Iterative depth-first search
     */
    for (std::size_t root_index{}; root_index < builder_count; ++root_index)
    {
        if (visit_states[root_index] != VisitState::eUnvisited)
        {
            continue;
        }

        visit_states[root_index] = VisitState::eInProgress;
        path.push_back(root_index);
        next_edge_indices.push_back(graph.node_edge_offsets[root_index]);

        while (!path.empty())
        {
            const std::size_t node_index{ path.back() };
            const std::size_t edge_index{ next_edge_indices.back() };

            if (edge_index == graph.node_edge_offsets[node_index + 1])
            {
                visit_states[node_index] = VisitState::eDone;
                path.pop_back();
                next_edge_indices.pop_back();
                continue;
            }

            ++next_edge_indices.back();

            switch (
                const std::size_t target{ graph.edge_target_indices[edge_index] };
                visit_states[target]
            )
            {
                case VisitState::eUnvisited:
                    visit_states[target] = VisitState::eInProgress;
                    path.push_back(target);
                    next_edge_indices.push_back(graph.node_edge_offsets[target]);
                    break;
                case VisitState::eInProgress:
                    report_cyclic_dependency(
                        std::span{ path }.subspan(
                            static_cast<std::size_t>(std::ranges::distance(
                                path.begin(),
                                std::ranges::find(path, target)
                            ))
                        ),
                        transient_memory_resource
                    );
                    return true;
                case VisitState::eDone: break;
            }
        }
    }

    return false;
}

auto EntryBuilderContainer::report_cyclic_dependency(
    const std::span<const std::size_t> cycle,
    std::pmr::memory_resource&         transient_memory_resource
) const -> void
{
    std::pmr::string path{ &transient_memory_resource };
    std::string_view subject_name;

    for (std::size_t position{ cycle.size() }; position > 0; --position)
    {
        const std::size_t index{ cycle[position % cycle.size()] };
        // ReSharper disable once CppTooWideScopeInitStatement
        const std::size_t next_index{ cycle[position - 1] };

        if (std::ranges::contains(
                m_builder_dependency_entry_hashes[next_index],
                m_entry_hashes[index]
            ))
        {
            if (path.empty())
            {
                subject_name = m_entry_names[next_index];
            }
            else
            {
                path.append(", ");
            }
            path.append(m_builder_names[index])
                .append(" <- ")
                .append(m_entry_names[next_index]);
        }
        else
        {
            if (path.empty())
            {
                subject_name = m_entry_names[index];
                path.append(subject_name);
            }
            path.append(" -> ").append(m_entry_names[next_index]);
        }
    }

    std::pmr::string message{
        "Cyclic dependency detected - entry of type `",
        &transient_memory_resource,
    };
    message.append(subject_name).append("` depends on itself (").append(path).append(")");

    PRECOND_AS(false, CyclicDependencyDetected, message);
}
#endif

}   // namespace redi
