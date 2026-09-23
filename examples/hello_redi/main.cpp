#include <cstdio>

import redi;

struct WindowSystem {};

struct RenderSystem {
    explicit RenderSystem(const redi::util::OptionalRef<WindowSystem> window_system)
        : window_system{ window_system }
    {
    }

    redi::util::OptionalRef<WindowSystem> window_system;
};

template <>
struct redi::EntryTraits<RenderSystem> {
    static auto describe_build(redi::BuildDirector<RenderSystem>& build_director) -> void
    {
        build_director.use_dependencies<redi::util::OptionalRef<WindowSystem>>();
    }
};

auto main() -> int
{
    /*
     * Registering `RenderSystem` automatically registers `WindowSystem`
     * as well when it is an unconditional dependency.
     */
    redi::Registry registry = redi::RegistryBuilder{}
                                  .register_entry<RenderSystem>()
                                  // order doesn't matter
                                  .register_entry<WindowSystem>()
                                  .build();

    /*
     * `RenderSystem` is never headless when `WindowSystem` is present
     */
    std::puts(
        registry.at<RenderSystem>().window_system.has_value()   //
            ? "Renderer is not headless"
            : "Renderer is headless"
    );
}
