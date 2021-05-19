// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#include "Vector3.hh"

#include <cmath>

#include "Math.hh"

using namespace tools;
using namespace std;

void Vector3::SetMagThetaPhi(double mag, double theta, double phi) {
    double amag = abs(mag);
    v[2] = amag * cos(theta);
    double rho(amag * sin(theta));
    v[1] = rho * sin(phi);
    v[0] = rho * cos(phi);
}

void Vector3::SetPtThetaPhi(double pt, double theta, double phi) {
    v[0] = pt * cos(phi);
    v[1] = pt * sin(phi);
    double tanTheta = tan(theta);
    v[2] = tanTheta ? pt / tanTheta : 0.;
}

void Vector3::SetRhoPhiZ(double rho, double phi, double z) {
    v[0] = rho * cos(phi);
    v[1] = rho * sin(phi);
    v[2] = z;
}

Vector3& Vector3::RotateX(double phi) {
    double sinphi = sin(phi);
    double cosphi = cos(phi);
    double ty = v[1] * cosphi - v[2] * sinphi;
    v[2] = v[2] * cosphi + v[1] * sinphi;
    v[1] = ty;
    return *this;
}

Vector3& Vector3::RotateY(double phi) {
    double sinphi = sin(phi);
    double cosphi = cos(phi);
    double tz = v[2] * cosphi - v[0] * sinphi;
    v[0] = v[0] * cosphi + v[2] * sinphi;
    v[2] = tz;
    return *this;
}

Vector3& Vector3::RotateZ(double phi) {
    double sinphi = sin(phi);
    double cosphi = cos(phi);
    double tx = v[0] * cosphi - v[1] * sinphi;
    v[1] = v[1] * cosphi + v[0] * sinphi;
    v[0] = tx;
    return *this;
}

Vector3& Vector3::Unit() {
    double norm = Math::norm2(v[0], v[1], v[2]);
    v[0] /= norm;
    v[1] /= norm;
    v[2] /= norm;
    return *this;
}
