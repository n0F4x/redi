# Tutorial

## The Registry

The registry can be thought of as a map of types and objects.

```c++
redi::Registry registry;

Entry& entry = registry.at<Entry>();
```

It can hold one entry per type.
This may be useful for storing context variables, or replacing singletons - and treating the registry as the source of truth.

## Entries

The registry works with entries.
Each type that satisfies the `redi::entry_c` concept may be used as an entry.

```c++
// entries must derive from `EntryBase`
struct Entry : redi::EntryBase {};

registry.emplace<Entry>();
```

### Describing dependencies

Entries must make it obvious what other entries they depend on, so that the library can determine the dependencies it needs to pull in when constructing them.

```c++
struct Entry;

auto describe_build(redi::BuildDirector<Entry>& director) -> void;

struct Entry : redi::BuildableEntry<Entry, describe_build> {};

auto make_entry(OtherEntry& other_entry) -> Entry;

auto describe_build(redi::BuildDirector<Entry>& director) -> void 
{
    director.use_function<make_entry>();   
}
```

One might think that this looks like too much boilerplate.
However, the library chooses to enforce dependency descriptions in a way that the resulting template instantiations may end up in one source file per declaration while also giving the highest degree of freedom to the user.
This dramatically increases compilation speed when used with deep dependency hierachies.

### Entry builders

Some entries require more setup than others.
Each entry type might decide to be built by a builder type.

```c++
struct Entry;

auto describe_build(redi::BuildDirector<Entry>& director) -> void;

struct Entry : redi::BuildableEntry<Entry, describe_build> {};

struct EntryBuilder : redi::EntryBuilderBase {
    auto build(OtherEntry& other_entry) -> Entry;
};

auto describe_build(redi::BuildDirector<Entry>& director) -> void
{
    director.use_builder<EntryBuilder>();
}
```

Entries can also depend on other builders, given that these builders are invoked later in the dependency graph.

```c++
struct EntryBuilder : redi::EntryBuilderBase {
    auto build(const OtherEntryBuilder& other_entry_builder) -> Entry;
};
```

### Builder builders

Similar to entries, builders may also depend on other builders.

```c++
struct EntryBuilder;

auto describe_build(redi::BuildDirector<EntryBuilder>& director) -> void;

struct EntryBuilder : redi::BuildableEntryBuilder<EntryBuilder, describe_build> {
    auto build() -> Entry;
};

auto make_entry_builder(OtherEntryBuilder& other_entry_builder) -> EntryBuilder;

auto describe_build(redi::BuildDirector<EntryBuilder>& director) -> void 
{
    director.use_function<make_entry_builder>();   
}
```

Notice how `make_entry_builder` takes a mutable reference to `OtherEntryBuilder`.
This is the way how builders are supposed to configure other builders.

`build` methods on the other hand must take other builders by `const&`.
This is to discourage `build` methods relying on other builders, as this pattern can easily disrupt the build order by creating cyclic dependencies when used incorrectly.

### Configuration entries

Configuration entries are special in the sense that they deliberately advertise the fact that they don't depend on any other entry.
This makes it ideal not only for other entries, but also builders to depend on them.

```c++
struct ConfigurationEntry : redi::ConfigurationEntry {};

auto make_entry_builder(ConfigurationEntry& configuration_entry) -> EntryBuilder;
```

## The Registry Builder

The registry builder is the point where everything comes together.
This is where entries are registered, and builders are built.

```c++
redi::RegistryBuilder registry_builder;

registry_builder.register_entry<Entry>();
registry_builder.register_entry<OtherEntry>();

redi::Registry registry = std::move(registry_builder).build();
```

- There is no `register_builder`.
  Builders get automatically picked up for entries that demand them.
- Entries may be registered multiple times.
  Only the first time counts.
- Registration order doesn't matter.

## Automatic dependency injection

Other dependency injection frameworks automate the process of resolving dependencies.
This library, however, goes one step further.
It not only resolves, but also builds each dependency!

```c++
/*
 * There is no need to register `DependencyEntry`
 *  as it gets automatically registered when
 *  `DependentEntry` is registered.
 */
registry_builder
    // .register_entry<DependencyEntry>()
    .register_entry<DependentEntry>();
```

This goes for any kind of unconditional dependency - be it _entry->entry_, _entry->builder_, _builder->builder_, or _builder->configuration_entry_.

## Optional dependencies

Certain entry types do not strictly require the presence of another entry, but would like to use them in case they are available.
This is when a given entry must advertise an optional dependency.

```c++
auto make_entry(redi::util::OptionalRef<const OtherEntry> optional_dependency) -> Entry;
```

All kinds of dependencies can be made optional.
For example a _builder to builder_ optional dependency looks like this:

```c++
auto make_entry_builder(redi::util::OptionalRef<const OtherEntryBuilder> optional_dependency) -> EntryBuilder;
```

### Sorting of optional dependencies

Automatically registering strict dependencies is trivial.
However, when it comes to optional dependencies, the library doesn't know ahead of time whether they are going to be present or not.

The best assumption to make is that the entry would want to be notified when an optional dependency is present, even if it got registered at a later point.
Therefore, the registry builder reorders the builders and the entries, so that optional dependencies are respected, just like regular dependencies.

## Pointer stability

Pointers/references to builders and entries can be stored safely.
The builders/entries themselves are not moved until the registry (or registry builder - in the case of builders) is destroyed.
Be cautious, however, that some builders might not be safe to use after their `build` methods have been called.

Entries are destroyed in reverse build order.
This makes it safe for entries to assume that their dependencies are still alive at the moment that they are destroyed.

```c++
struct EntryA {
    ~EntryA() { std::println("Last to be destroyed"); }
};

struct EntryB {
    const EntryA* a;

    explicit EntryB(const EntryA& a);
    ~EntryB() { std::println("Gets destroyed before `EntryA`"); }
};
```

Builders are destroyed in an undetermined order.
