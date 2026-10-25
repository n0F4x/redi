export module redi.opted_into_entry_concept_c;

import redi.opted_into_entry_builder_concept_c;

namespace redi {

/*
 * All non-entry-builder types are implicitly opted into the entry concept.
 */
export template <typename T>
concept opted_into_entry_concept_c = !opted_into_entry_builder_concept_c<T>;

}   // namespace redi
