# How to build the project

## Prerequisites

Make sure you have the following programs installed on your machine

-   Clang (at least version 22)
-   Ninja
-   CMake
-   [Conan](https://docs.conan.io/2/installation.html)

## Setup

### Create a Conan [profile](https://docs.conan.io/2/reference/config_files/profiles.html#profiles)

#### Example Conan profile for Windows:

```ini
[settings]
os=Windows
arch=x86_64
build_type=Debug
compiler=clang
compiler.cppstd=23
compiler.version=22
compiler.runtime=dynamic
compiler.runtime_type=Debug
compiler.runtime_version=v145

[conf]
tools.build:compiler_executables={'c':'C:/Program Files/LLVM/bin/clang.exe','cpp':'C:/Program Files/LLVM/bin/clang++.exe'}
tools.cmake:cmake_program="C:/Program Files/CMake/bin/cmake.exe"
tools.cmake.cmaketoolchain:generator=Ninja
user.redi:dev=True
user.redi:debug=True
user.redi:enable_tests=True
user.redi:enable_examples=True

[buildenv]
PATH+=(path)C:/Program Files/LLVM/bin
PATH+=(path)C:/Program Files/Ninja

[platform_tool_requires]
cmake/4.3
```

#### Example Conan profile for MSYS2 (Linux is very similar):

```ini
[settings]
os=Windows
os.subsystem=msys2
arch=x86_64
build_type=Debug
compiler=clang
compiler.cppstd=23
compiler.version=22
compiler.libcxx=libc++

[conf]
tools.build:compiler_executables={'c':'C:/msys64/clang64/bin/clang.exe','cpp':'C:/msys64/clang64/bin/clang++.exe'}
tools.cmake.cmaketoolchain:generator=Ninja
user.redi:dev=True
user.redi:debug=True
user.redi:enable_tests=True
user.redi:enable_examples=True

[buildenv]
PATH+=(path)C:/msys64/clang64/bin

[runenv]
PATH+=(path)C:/dev/msys64/clang64/bin

[platform_tool_requires]
cmake/4.3
```

### Install the dependencies

Run `conan install . -b=missing --profile=<custom_profile>`.

This will produce a _CMakeUserPresets.json_ file inside the working directory that can be used to build the project (with your IDE).

## Caveats

### Conan dev variables

Project local variables can be set using a Conan profile like this:

```ini
[conf]
user.redi:<variable_name>=<value>
```

The following variables/toggles are used by the project:

* `dev=True`- Tells the package to configure itself for development.
    This also enables the CMakeLists.txt found in the root directory.
    All other options below are dependent on this.
    When `dev` is not set to `True`, the rest of the development options won't have an effect.
  
* `debug=True` - Enables extra functionalities useful for debugging.

* `enable_tests=True` - Enables building the tests.
    This flag will also define the `REDI_TEST` macro, which makes our contract violation handlers throw a custom exception (that the tests can catch) instead of printing a nice error message and stopping the debugger / terminating.

* `enable_examples=True` - Enables building the examples.

### Project local Conan profile

You can place your Conan profile file inside the project directory (e.g. _./.conan2/profiles/customprofile_).
Use the relative path to your profile when executing Conan commands.

### CLion set toolchain

CLion is a bit weird when it comes to build profiles using cmake-presets.
If you can't change the toolchain for the profile, make a copy of it.

### Test the library as a package

It is important that the library can also be consumed as a third-party package.

Run `conan export .` to export the package to the local cache.

Run `conan test redi/<version> -b=missing --profile=<custom_test_profile>` to test the package in the local cache.

Make sure not to declare `user.redi:dev` as `True` in your Conan profile used for testing.
