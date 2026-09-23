module;

#include <concepts>

export module redi.describes_entry_build_c;

import redi.BuildDirector;
import redi.entry_c;

namespace redi {

export template <typename T, typename Entry_T>
concept describes_entry_build_c
    = entry_c<Entry_T> && std::invocable<T, BuildDirector<Entry_T>&>;

}   // namespace redi
