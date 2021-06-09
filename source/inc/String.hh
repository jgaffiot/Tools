// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef STRING_HH
#define STRING_HH 1

#include <cstring>
#include <sstream>
#include <string>
#include <utility>

namespace tools
{
namespace detail_string
{
template<typename T>
inline void PushToStream(std::ostringstream& oss, T&& val) {
    oss << std::forward<T>(val);
}

template<typename T, typename... Args>
inline void PushToStream(std::ostringstream& oss, T&& val, Args&&... args) {
    oss << std::forward<T>(val);
    PushToStream(oss, std::forward<Args>(args)...);
}
}  // end namespace detail_string

template<typename... Args>
inline const char* cat(Args&&... args) {
    std::ostringstream oss;
    detail_string::PushToStream(oss, std::forward<Args>(args)...);
    size_t len = std::strlen(oss.str().c_str()) + 1;
    char* new_str = new char[len];
    std::snprintf(new_str, len, "%s", oss.str().c_str());
    return new_str;
}

template<typename... Args>
inline std::string scat(Args&&... args) {
    std::ostringstream oss;
    detail_string::PushToStream(oss, std::forward<Args>(args)...);
    return oss.str();
}

// stuff for emulation of switch with strings
typedef std::uint64_t hash_t;
namespace detail_string
{
constexpr static hash_t prime = 0x100000001B3ull;
constexpr static hash_t basis = 0xCBF29CE484222325ull;
}  // namespace detail_string
constexpr hash_t HashCompileTime(
    const char* const str, hash_t last_value = detail_string::basis) {
    return *str ? HashCompileTime(str + 1, (*str ^ last_value) * detail_string::prime)
                : last_value;
}
constexpr hash_t operator"" _hash(const char* const p, size_t) {
    return HashCompileTime(p);
}
hash_t HashRunTime(char const* str);

}  // namespace tools

#endif  // STRING_HH
