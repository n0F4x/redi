module;

#include <print>

module redi.util.contracts;

namespace redi::util {

auto PreconditionViolation::print() const -> void
{
    std::println("{}", headline());
    std::println(
        "    {} {} ({}:{})",
        "source:",
        m_location.file_name(),
        m_location.line(),
        m_location.column()
    );
    std::println("    {} {}", "function:", m_location.function_name());
    std::println("    {} {}", "condition:", m_condition_as_string);
    std::println("    {} {}", "message:", m_message);
}

}   // namespace redi::util
