// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef MATH_HH
#define MATH_HH 1

#include <cmath>

namespace tools
{
namespace Math
{
////////////////////////////////////////////////////////////////
///////////////// Generic mathematical constant ////////////////
////////////////////////////////////////////////////////////////
// Some mathematical constants, taken from TMath.hh
inline constexpr double Pi() {
    return 3.14159265358979323846264338327950288419716939937510;
}  // 50 digits
inline constexpr double TwoPi() {
    return 2.0 * Pi();
}
inline constexpr double PiSq() {
    return Pi() * Pi();
}
inline constexpr double PiOver2() {
    return Pi() / 2.0;
}
inline constexpr double PiOver4() {
    return Pi() / 4.0;
}
inline constexpr double InvPi() {
    return 1.0 / Pi();
}
inline constexpr double RadToDeg() {
    return 180.0 / Pi();
}
inline constexpr double DegToRad() {
    return Pi() / 180.0;
}
inline constexpr double Sqrt2() {
    return 1.4142135623730950488016887242097;
}

////////////////////////////////////////////////////////////////
///////////////// Generic mathematical function ////////////////
////////////////////////////////////////////////////////////////
// Some mathematical functions with unlimited range thanks to variadic templates

template<typename T>
inline T sq(T val) {
    return val * val;
}
template<typename T>
inline T square(T val) {
    return val * val;
}
inline unsigned long long int operator"" _sq(unsigned long long int val) {
    return val * val;
}
inline long double operator"" _sq(long double val) {
    return val * val;
}

template<typename T>
inline T cube(T val) {
    return val * val * val;
}
inline unsigned long long int operator"" _cube(unsigned long long int val) {
    return val * val * val;
}
inline long double operator"" _cube(long double val) {
    return val * val * val;
}

template<typename T>
inline T pow_n(T base, unsigned int exp) {
    if (exp == 0u) {
        return static_cast<T>(1);
    }
    T result = static_cast<T>(1);
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

/// Sum of squares, result given in double
template<typename T>
inline double sum2(T val) {
    return static_cast<double>(val) * static_cast<double>(val);
}

template<typename T, typename... Args>
inline double sum2(T val, Args... args) {
    return static_cast<double>(val) * static_cast<double>(val) + sum2(args...);
}

/// Norm 2, the square root of the sum of squares, result given in double
template<typename... Args>
inline double norm2(Args... args) {
    return std::sqrt(sum2(args...));
}

/// Difference of square
template<typename T, typename U>
inline double diff2(T a, U b) {
    return static_cast<double>(a) * static_cast<double>(a)
           - static_cast<double>(b) * static_cast<double>(b);
}

}  // end namespace Math

}  // end namespace tools

#endif  // MATH_HH
