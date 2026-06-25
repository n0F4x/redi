# `redi` - Registry pattern meets Dependency Injection

## Overview

`redi` is another dependency injection library focusing on providing a great developer experience.

```c++
import redi;

// ... your own imports

int main()
{
    /*
     * Registering `RenderSystem` automatically registers `WindowSystem`
     * as well when it is an unconditional dependency.
     */
    redi::Registry registry = redi::RegistryBuilder{}
                                  // order doesn't matter
                                  .register_entry<RenderSystem>()
                                  .register_entry<WindowSystem>()
                                  .build();
    
    /*
     * `RenderSystem` is never headless when `WindowSystem` is present
     */
    std::puts(
        registry.at<RenderSystem>().window_system == nullptr   //
            ? "Renderer is headless"
            : "Renderer is not headless"
    );
}
```

### Key Features

- Automatic dependency injection
    - Dependencies are automatically registered.
      You only need to register the concrete types you use.
- Optional dependencies
    - Feel free to describe all the optional dependencies your types may need.
      They will be delivered when they are present.
- Support for configurable builder objects
    - Use the builder method for complex building patterns.
- Elegant error messages
    ```
    Cyclic dependency detected - entry of type `WindowSystem` depends on itself (WindowSystem -> RenderSystem -> WindowSystem)
    ```

## Tutorial

Read the [tutorial](./docs/Tutorial.md) for an in-depth guide on how to inject dependencies using the library.

## Installation

The library has no dependencies.
You can include it as a subdirectory, use `FetchContent`, or use [Conan](https://docs.conan.io/2/index.html).

Read more about the specific Conan workflow [here](./docs/Building.md).
