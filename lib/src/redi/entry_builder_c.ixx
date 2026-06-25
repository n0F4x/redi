module;

#include <concepts>
#include <type_traits>

export module redi.entry_builder_c;

import redi.BuildableEntryBuilderBase;
import redi.configuration_entry_c;
import redi.entry_c;
import redi.EntryBase;
import redi.EntryBuilderBase;
import redi.represents_entry_dependency_c;
import redi.util.concepts.storable;
import redi.util.containers.OptionalRef;
import redi.util.type_traits.result_of;

namespace redi {

template <typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (*)(Dependencies_T...)->Entry_T) -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...)->Entry_T) -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...) const->Entry_T)
    -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...) &->Entry_T)
    -> bool = delete;

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...) const&->Entry_T)
    -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...) &&->Entry_T)
    -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

template <typename Builder_T, typename... Dependencies_T, typename Entry_T>
consteval auto is_entry_builder(auto (Builder_T::*)(Dependencies_T...) const&&->Entry_T)
    -> bool
{
    return (represents_entry_dependency_c<Dependencies_T> && ...) && entry_c<Entry_T>;
}

export template <typename T>
concept entry_builder_c = util::storable_c<T>
                       && std::derived_from<T, EntryBuilderBase>
                       && requires { requires is_entry_builder(&T::build); }
                       && (std::default_initializable<T>
                           || std::derived_from<T, internal::BuildableEntryBuilderBase>)
                       && !std::derived_from<T, EntryBase>;

export template <typename T>
concept decays_to_entry_builder_c = entry_builder_c<std::decay_t<T>>;

}   // namespace redi
