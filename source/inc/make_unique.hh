// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef MAKE_UNIQUE_HH
#define MAKE_UNIQUE_HH 1

#include <memory>

// make_unique is not part of C++11 (why ???), but is at least included in C++14

// Define make_unique only for C++11
#if __cplusplus < 201103L
#    error C++11 or higher is required
#elif __cplusplus == 201103L

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// with deleter
template<typename T, typename D, typename... Args>
std::unique_ptr<T, D> make_unique(Args&&... args) {
    return std::unique_ptr<T, D>(new T(std::forward<Args>(args)...));
}

#endif  // __cplusplus == 201103L

#endif  // MAKE_UNIQUE_HH
