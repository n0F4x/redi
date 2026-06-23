export module redi.CyclicDependencyDetected;

import redi.util.contracts;

namespace redi {

export class CyclicDependencyDetected
    : public util::PreconditionViolation   //
{
    using PreconditionViolation::PreconditionViolation;
};

}   // namespace redi
