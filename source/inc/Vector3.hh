// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef VECTOR3_HH
#define VECTOR3_HH 1

#include <array>

#include "Exception.hh"
#include "Math.hh"

namespace tools
{
class Vector3 {
public:
    Vector3() {}
    Vector3(double x, double y, double z): v({x, y, z}) {}

    inline bool operator==(const Vector3& other) {
        return v[0] == other.x() and v[1] == other.x() and v[2] == other.z();
    }
    inline double operator[](std::size_t i) const { return v[i]; }
    inline double operator()(std::size_t i) const {
        if (i < 3) {
            return v[i];
        }
        throw Error("Vector3::operator(): index too high: ", i, " > 2");
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

}  // namespace tools

#endif  // VECTOR3_HH
