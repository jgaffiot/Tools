// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * FROM: Howard Hinnant, 康桓瑋, Val, einpoklum
 * See https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
 * DATE: 2021-07-22
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef DISPLAY_HH
#define DISPLAY_HH 1

#include <iostream>
#include <string_view>

namespace tools
{
template<typename T>
constexpr std::string_view type_name();

template<>
constexpr std::string_view type_name<void>() {
    return "void";
}

namespace detail_display
{
using type_name_prober = void;

template<typename T>
constexpr std::string_view wrapped_type_name() {
#ifdef __clang__
    return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
#    error "Unsupported compiler"
#endif
}

constexpr std::size_t wrapped_type_name_prefix_length() {
    return wrapped_type_name<type_name_prober>().find(type_name<type_name_prober>());
}

constexpr std::size_t wrapped_type_name_suffix_length() {
    return wrapped_type_name<type_name_prober>().length()
           - wrapped_type_name_prefix_length() - type_name<type_name_prober>().length();
}

}  // namespace detail_display

//! Return the type name as a string, to log or debug
template<typename T>
constexpr std::string_view type_name() {
    constexpr auto wrapped_name = detail_display::wrapped_type_name<T>();
    constexpr auto prefix_length = detail_display::wrapped_type_name_prefix_length();
    constexpr auto suffix_length = detail_display::wrapped_type_name_suffix_length();
    constexpr auto type_name_length =
        wrapped_name.length() - prefix_length - suffix_length;
    return wrapped_name.substr(prefix_length, type_name_length);
}

//! An operator<< on all enum types, enabling easy printing to log or debug
template<typename Enum>
inline std::ostream& operator<<(
    typename std::enable_if<std::is_enum<Enum>::value, std::ostream>::type& stream,
    const Enum& e) {
    return stream << static_cast<typename std::underlying_type<Enum>::type>(e);
}

}  // namespace tools

#endif  // DISPLAY_HH
