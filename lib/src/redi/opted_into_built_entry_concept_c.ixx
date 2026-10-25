export module redi.opted_into_built_entry_concept_c;

import redi.BuildDirector;

namespace redi {

export template <typename T>
concept opted_into_built_entry_concept_c = internal::opted_into_built_entry_concept_c<T>;

}   // namespace redi
