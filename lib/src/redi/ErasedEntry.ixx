export module redi.ErasedEntry;

import redi.entry_c;
import redi.util.containers.MoveOnlyAny;

namespace redi {

template <typename T>
struct EntryConcept {
    constexpr static bool value{ entry_c<T> };
};

export using ErasedEntry
    = util::BasicMoveOnlyAny<0, util::default_any_alignment(), EntryConcept>;

}   // namespace redi
