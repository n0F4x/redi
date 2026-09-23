export module redi.EntryTraits;

import redi.entry_c;

namespace redi {

/*
 * Customization points:
 *      - `static auto describe_build(BuildDirector<Entry_T>& build_director) -> void`
 *      - `constexpr static bool is_configuration_entry`
 */
export template <entry_c Entry_T>
struct EntryTraits;

}   // namespace redi
