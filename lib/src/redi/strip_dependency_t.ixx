module;

#include <type_traits>

export module redi.strip_dependency_t;

import redi.util.containers.OptionalRef;

namespace redi {

template <typename T>
struct StripDependency {
    using type = std::remove_cvref_t<T>;
};

template <typename T>
struct StripDependency<util::OptionalRef<T>> {
    using type = std::remove_const_t<T>;
};

export template <typename T>
using strip_dependency_t = StripDependency<T>::type;

}   // namespace redi
