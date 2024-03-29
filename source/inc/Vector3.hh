// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef TOOLS_VECTOR3_HH
#define TOOLS_VECTOR3_HH 1

#include <array>
#include <ostream>
#include <string>
#include <type_traits>

#include "BaseError.hh"
#include "Math.hh"

namespace tools
{
class Vector3Error: public BaseError {
public:
    explicit Vector3Error(const std::string& arg): BaseError(arg) {}
    explicit Vector3Error(const std::string& arg, std::ofstream& write):
        BaseError(arg, write) {}

    template<typename T>
    explicit Vector3Error(const T& val): BaseError(val) {}
    template<typename T, typename... Args>
    Vector3Error(const T& val, Args... args): BaseError(val, args...) {}
};

class Vector3 {
public:
    Vector3() {}
    Vector3(double x, double y, double z): v({x, y, z}) {}

    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;

    inline double operator[](std::size_t i) const { return v[i]; }
    inline double operator()(std::size_t i) const {
        if (i < 3) {
            return v[i];
        }
        throw Vector3Error("Vector3::operator(): index too high: ", i, " > 2");
    }
    inline double at(std::size_t i) const { return v.at(i); }
    inline double x() const { return v[0]; }
    inline double y() const { return v[1]; }
    inline double z() const { return v[2]; }

    Vector3& Unit();
    inline double Norm() const { return Math::norm2(v[0], v[1], v[2]); }
    inline double Mag() const { return Math::norm2(v[0], v[1], v[2]); }
    inline double Mag2() const { return Math::sum2(v[0], v[1], v[2]); }

    inline void SetX(double x) { v[0] = x; }
    inline void SetY(double y) { v[1] = y; }
    inline void SetZ(double z) { v[2] = z; }
    inline void SetXYZ(double x, double y, double z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    void SetMagThetaPhi(double mag, double theta, double phi);
    void SetPtThetaPhi(double pt, double theta, double phi);
    void SetRhoPhiZ(double rho, double phi, double z);

    Vector3& RotateX(double phi);
    Vector3& RotateY(double phi);
    Vector3& RotateZ(double phi);

    friend Vector3 operator+(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a, const Vector3& b);
    friend double operator*(const Vector3& a, const Vector3& b);
    friend Vector3 operator*(const Vector3& v, double d);
    friend Vector3 operator*(double d, const Vector3& v);
    friend Vector3 operator/(const Vector3& v, double d);
    friend bool operator==(const Vector3& a, const Vector3& b);
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);

private:
    std::array<double, 3> v = {{}};
};

inline Vector3 operator+(const Vector3& a, const Vector3& b) {
    return Vector3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}
inline Vector3 operator-(const Vector3& a, const Vector3& b) {
    return Vector3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}
inline double operator*(const Vector3& a, const Vector3& b) {
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
inline Vector3 operator*(const Vector3& v, double d) {
    return Vector3(d * v.x(), d * v.y(), d * v.z());
}
inline Vector3 operator*(double d, const Vector3& v) {
    return Vector3(d * v.x(), d * v.y(), d * v.z());
}
inline Vector3 operator/(const Vector3& v, double d) {
    return Vector3(v.x() / d, v.y() / d, v.z() / d);
}
inline bool operator==(const Vector3& a, const Vector3& b) {
    return Math::equals(a.x(), b.x()) and Math::equals(a.y(), b.y())
           and Math::equals(a.z(), b.z());
}

std::ostream& operator<<(std::ostream& os, const Vector3& v);

}  // namespace tools

#endif  // TOOLS_VECTOR3_HH
