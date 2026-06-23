from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Version


class RediRecipe(ConanFile):
    name = "redi"
    version = "0.0.0"
    package_type = "library"

    # Optional metadata
    url = "https://github.com/n0f4x/redi"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "debug": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "debug": False,
    }
    implements = ["auto_shared_fpic"]
    exports_sources = (
        "lib/*",
    )

    project_prefix = "REDI_"

    @property
    def _dev(self):
        return bool(self.conf.get(f"user.{self.name}:dev", default=False))

    @property
    def _debug(self):
        return self.options.debug or (
                self._dev and bool(self.conf.get(f"user.{self.name}:debug", default=True)))

    @property
    def _enable_tests(self):
        return self._dev and bool(self.conf.get(f"user.{self.name}:enable_tests", default=False))

    @property
    def _enable_examples(self):
        return self._dev and bool(self.conf.get(f"user.{self.name}:enable_examples", default=False))

    def validate(self):
        check_min_cppstd(self, "26")

        supported_compilers = ["clang"]
        if self.settings.compiler not in supported_compilers:
            raise ConanInvalidConfiguration(
                f"{self.settings.compiler} is not supported."
                f"Supported compilers are: {supported_compilers}"
            )

        if self.settings.compiler == "clang":
            minimum_supported_clang_version = 22
            if self.settings.compiler.version < Version(minimum_supported_clang_version):
                raise ConanInvalidConfiguration(
                    f"Clang versions below {minimum_supported_clang_version} are not supported"
                )

        if self.conf.get("tools.cmake.cmaketoolchain:generator") != "Ninja":
            raise ConanInvalidConfiguration("Only Ninja is supported as a generator")

        if bool(self.conf.get(f"user.{self.name}:debug", default=False)) and not self._dev:
            raise ConanInvalidConfiguration(
                f"'user.{self.name}:debug' requires 'user.{self.name}:dev'"
            )

        if bool(self.conf.get(f"user.{self.name}:enable_tests", default=False)) and not self._dev:
            raise ConanInvalidConfiguration(
                f"'user.{self.name}:enable_tests' requires 'user.{self.name}:dev'"
            )

        if bool(self.conf.get(f"user.{self.name}:enable_examples", default=False)) and not self._dev:
            raise ConanInvalidConfiguration(
                f"'user.{self.name}:enable_examples' requires 'user.{self.name}:dev'"
            )

    def build_requirements(self):
        self.tool_requires("cmake/[>=4.3]")

    def requirements(self):
        if self._enable_tests:
            self.test_requires("catch2/3.15.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        # CMakeDeps
        deps = CMakeDeps(self)
        deps.generate()

        # CMakeToolChain
        tc = CMakeToolchain(self)

        if self._dev:
            tc.cache_variables[self.project_prefix + "DEBUG"] = self._debug
            tc.cache_variables[self.project_prefix + "ENABLE_TESTS"] = self._enable_tests
            tc.cache_variables[self.project_prefix + "ENABLE_EXAMPLES"] = self._enable_examples

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=None if self._dev else "lib")
        cmake.build()
        if self._enable_tests and not self.conf.get("tools.build:skip_test", default=False):
            cmake.ctest(cli_args=["--output-on-failure"])

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "redi")
        self.cpp_info.set_property("cmake_target_name", "redi::redi")

        version = Version(self.version)
        self.cpp_info.defines.append(f"{self.project_prefix}VERSION_MAJOR={version.major}")
        self.cpp_info.defines.append(f"{self.project_prefix}VERSION_MINOR={version.minor}")
        self.cpp_info.defines.append(f"{self.project_prefix}VERSION_PATCH={version.patch}")

        if self.options.debug:
            self.cpp_info.defines.append(f"{self.project_prefix}DEBUG")

        # TODO: remove these once Conan learns cxx modules
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.builddirs = ["."]
