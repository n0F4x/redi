module;

#include <stdexcept>

export module redi.EntryBuilderFailedError;

namespace redi {

export class EntryBuilderFailedError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

}   // namespace redi
