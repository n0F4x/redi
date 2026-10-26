#include <concepts>
#include <cstdio>
#include <utility>

import redi;

struct BuiltEntry {};

template <std::derived_from<BuiltEntry> Entry_T>
    requires redi::entry_c<Entry_T>
struct redi::EntryTraits<Entry_T> {
    constexpr static auto describe_build(BuildDirector<Entry_T>& build_director) -> void
    {
        build_director.template use_builder<typename Entry_T::Builder>();
    }
};

struct GraphicsSystemIntegration {};

struct WindowSystem : BuiltEntry {
    struct Builder;

    GraphicsSystemIntegration* graphics_system{};
};

struct WindowSystem::Builder {
    using BuilderConcept = redi::EntryBuilderTag;

    bool graphics_support_requested = false;

    auto build(
        const redi::util::OptionalRef<GraphicsSystemIntegration> graphics_system_integration
    ) const -> WindowSystem
    {
        if (graphics_support_requested && graphics_system_integration.has_value())
        {
            return WindowSystem{ .graphics_system = &*graphics_system_integration };
        }
        return WindowSystem{};
    }
};

struct RenderSystem : BuiltEntry {
    struct Builder;

    GraphicsSystemIntegration& graphics_system;
    WindowSystem*              window_system{};
};

struct RenderSystem::Builder {
    using BuilderConcept = redi::BuiltEntryBuilderTag;

    static auto create(
        const redi::util::OptionalRef<WindowSystem::Builder> window_builder
    ) -> Builder
    {
        if (window_builder.has_value())
        {
            window_builder->graphics_support_requested = true;
        }

        return Builder{};
    }

    static auto build(
        GraphicsSystemIntegration&                  graphics_system_integration,
        const redi::util::OptionalRef<WindowSystem> window_system
    ) -> RenderSystem
    {
        return RenderSystem{
            .graphics_system = graphics_system_integration,
            .window_system   = window_system.has_value() ? &*window_system : nullptr,
        };
    }

private:
    friend auto describe_build(redi::BuildDirector<Builder>& build_director) -> void
    {
        build_director.use_function<create>();
    }
};

auto main() -> int
{
    redi::RegistryBuilder registry_builder;

    /*
     * `GraphicsSystemIntegration` gets implicitly injected
     *  as `RenderSystem` unconditionally depends on it.
     * `WindowSystem` gets built before `RenderSystem`,
     *  as `RenderSystem` (conditionally) depends on it.
     */
    redi::Registry registry = std::move(registry_builder)
                                  .register_entry<RenderSystem>()
                                  .register_entry<WindowSystem>()
                                  .build();

    /*
     * `RenderSystem` is never headless when `WindowSystem` is present
     */
    std::puts(
        registry.at<RenderSystem>().window_system == nullptr ? "Renderer is headless"
                                                             : "Renderer is not headless"
    );
}
