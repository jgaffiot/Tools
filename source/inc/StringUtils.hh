// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef TOOLS_STRING_UTILS_HH
#define TOOLS_STRING_UTILS_HH 1

#include <cstring>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace tools
{
namespace detail_string
{
template<typename S, typename T, typename = void>
struct is_streamable: std::false_type {};

template<typename S, typename T>
struct is_streamable<
    S,
    T,
    std::void_t<decltype(std::declval<S&>() << std::declval<T>())>>: std::true_type {};

template<
    typename T,
    typename = typename std::enable_if<is_streamable<std::ostringstream, T>{}>::type>
inline void PushToStream(std::ostringstream& oss, T&& val) {
    oss << std::forward<T>(val);
}

template<
    typename T,
    typename... Args,
    typename = typename std::enable_if<is_streamable<std::ostringstream, T>{}>::type>
inline void PushToStream(std::ostringstream& oss, T&& val, Args&&... args) {
    oss << std::forward<T>(val);
    PushToStream(oss, std::forward<Args>(args)...);
}

template<typename C, typename D, typename T = typename C::value_type>
inline std::string JoinImpl(const C& container, const D& delimiter) {
    size_t n = 0UL;
    std::ostringstream oss;
    for (auto& el : container) {
        if (n == 0UL) {
            oss << el;
        } else {
            oss << delimiter << el;
        }
        n++;
    }
    return oss.str();
}
}  // end namespace detail_string

//! Concatenation of any streamable objects into a string, returns a C-string
template<typename... Args>
inline const char* ccat(Args&&... args) {
    std::ostringstream oss;
    detail_string::PushToStream(oss, std::forward<Args>(args)...);
    size_t len = std::strlen(oss.str().c_str()) + 1;
    char* new_str = new char[len];
    std::snprintf(new_str, len, "%s", oss.str().c_str());
    return new_str;
}

//! Concatenation of any streamable objects into a string, returns a std::string
template<typename... Args>
inline std::string cat(Args&&... args) {
    std::ostringstream oss;
    detail_string::PushToStream(oss, std::forward<Args>(args)...);
    return oss.str();
}

//! Simple join of containers into a string, with a char delimiter
template<
    typename C,
    typename T = typename C::value_type,
    typename = typename std::enable_if<
        detail_string::is_streamable<std::ostringstream, T>{}>::type>
inline std::string join(C const& container, char delimiter = ',') {
    return detail_string::JoinImpl(container, delimiter);
}

//! Simple join of containers into a string, with a string delimiter
template<
    typename C,
    typename T = typename C::value_type,
    typename = typename std::enable_if<
        detail_string::is_streamable<std::ostringstream, T>{}>::type>
inline std::string join(C const& container, const std::string& delimiter = ", ") {
    return detail_string::JoinImpl(container, delimiter);
}

//! Simple splitting of a string, over a char
inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(s);
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

//! Simple splitting of a string, over a string
inline std::vector<std::string> split(
    const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos_start = 0UL;
    size_t pos_end = s.find(delimiter);
    size_t delim_len = delimiter.length();

    while (pos_end != std::string::npos) {
        tokens.push_back(s.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + delim_len;
        pos_end = s.find(delimiter, pos_start);
    }
    tokens.push_back(s.substr(pos_start));
    return tokens;
}

// String constexpr hash function to allow the use of "switch" with strings
//! Return type of the string hash function
using hash_t = std::uint64_t;

namespace detail_string
{
constexpr static hash_t prime = 0x100000001B3ull;
constexpr static hash_t basis = 0xCBF29CE484222325ull;
}  // namespace detail_string

//! constexpr hash function
constexpr hash_t strhash(std::string_view str, hash_t last_val = detail_string::basis) {
    return not str.empty() ? strhash(
               str.data() + 1, (str.front() ^ last_val) * detail_string::prime)
                           : last_val;
}
//! operator _hash intended for constexpr hash of C-string
constexpr hash_t operator"" _hash(const char* const p, size_t) {
    return strhash(p);
}

}  // namespace tools

#endif  // TOOLS_STRING_UTILS_HH
