module;

#include <print>

module redi.util.contracts;

namespace redi::util {

auto PreconditionViolation::print() const -> void
{
    std::println(stderr, "{}", headline());
    std::println(
        stderr,
        "    {} {}:{}",
        "source:",
        m_location.file_name(),
        m_location.line()
    );
    std::println(stderr, "    {} {}", "function:", m_location.function_name());
    std::println(stderr, "    {} {}", "condition:", m_condition_as_string);
    std::println(stderr, "    {} {}", "message:", m_message);
}

}   // namespace redi::util
