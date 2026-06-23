module;

#include <concepts>
#include <format>
#if __has_cpp_attribute(__cpp_lib_debugging)
  #include <debugging>
#endif
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>

export module redi.util.contracts;

namespace redi::util {

export class PreconditionViolation : public std::logic_error {
public:
    constexpr PreconditionViolation(
        const std::source_location& location,
        const std::string_view      condition_as_string,
        const std::string_view      message
    )
        : std::logic_error{
              std::format("{}\n{}\n{}", headline(), condition_as_string, message)
          },
          m_location{ location },
          m_condition_as_string{ condition_as_string },
          m_message{ message }
    {
    }

    auto print() const -> void;

private:
    std::source_location m_location;
    std::string          m_condition_as_string;
    std::string          m_message;

    [[nodiscard]]
    constexpr static auto headline() -> std::string_view
    {
        constexpr static std::string_view result{ "Precondition violated!" };
        return result;
    }
};

export template <typename Exception_T = PreconditionViolation>
    requires std::same_as<Exception_T, PreconditionViolation>
          || std::derived_from<Exception_T, PreconditionViolation>
constexpr auto propagate_precondition_violation(
    const std::string_view      condition_as_string,
    const std::source_location& location,
    const std::string_view      message = ""
) -> void
{
    const Exception_T precondition_violation{
        location,
        condition_as_string,
        message,
    };

#ifdef REDI_TEST
    throw auto{ precondition_violation };
#endif

    precondition_violation.print();

#if __has_cpp_attribute(__cpp_lib_debugging)
    std::breakpoint();
#else
  #ifdef _MSC_VER
    __debugbreak();
  #else
    #if __has_builtin(__builtin_debugtrap)
    __builtin_debugtrap();
    #else
    __builtin_trap();
    #endif
  #endif
#endif
}

}   // namespace redi::util
