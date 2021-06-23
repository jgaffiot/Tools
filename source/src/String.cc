// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#include "String.hh"

namespace tools
{
hash_t HashRunTime(const char* str) {
    hash_t ret{detail_string::basis};
    while (*str) {
        ret ^= *str;
        ret *= detail_string::prime;
        ++str;
    }
    return ret;
}

}  // namespace tools
