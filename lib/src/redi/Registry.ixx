export module redi.Registry;

import redi.ErasedEntry;
import redi.util.containers.GenericStack;

namespace redi {

export using Registry = util::BasicGenericStack<ErasedEntry>;

}   // namespace redi
